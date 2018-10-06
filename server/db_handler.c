
#include "db_handler.h"

data_base* Create_data_base() {
	data_base* new_base = NULL;
	new_base = init_data_base();
	if (new_base != NULL) {
		new_base->binary_tree = create_tree(&compare_uu, &compare_elements_hess);
	}
	return new_base;
}


req_status Create_table(data_base* _db, const char* _name) {
	req_status result;
	db_element* new_element = NULL;
	result.status = UNDEFINED;
	if(_db != NULL && _name != NULL){
		new_element = init_db_element();
		if(new_element != NULL) {
			new_element->table = create_h_table(_name, &compare_ss, &hash_str, &get_element_value_s, &create_element_ss);
			if(_db->data_head == NULL) {
				new_element->head = &_db->data_head;
				_db->data_head = new_element;
			} else {
				new_element->head = &_db->data_head;
				new_element->next = _db->data_head;
				new_element->next->prev = new_element;
				_db->data_head = new_element;
			}
			result.status = TABLE_CREATED;
		}
	}
	return result;
}

req_status Delete_table(data_base* _db, const char* _name) {
	req_status result;
	db_element* curr_element = NULL;
	result.status = UNDEFINED;
	if(_db != NULL && _name != NULL) {
		result.status = TABLE_NOT_EXISTS;
		curr_element = find_db_table(_db, _name);
		if (curr_element != NULL) {
			remove_db_table(_db, curr_element);
			result.status = TABLE_REMOVED;
		}
	} 
	return result;
}

req_status Update_table(data_base* _db, const char* _name, const void* _key, const void* _value, unsigned long long int _ttl) {
	req_status result;
	ht_element* curr_element = NULL;
	db_element* data = NULL;
	p_data search_result;
	result.status = UNDEFINED;
	if(_db != NULL && _name != NULL && _key != NULL && _value != NULL) {
		result.status = TABLE_NOT_EXISTS;
		data = find_db_table(_db, _name);
		if(data != NULL) {
			result.status = TABLE_ELEMENT_NOT_EXISTS;
			search_result = find_ht_element(data->table, _key);
			if(search_result.current != NULL) { // same key-element exists
				result.status = TABLE_ELEMENT_FOUND;
				update_db_element_data(_db, search_result.current, _key, _value, translate_time_to_key(_ttl));
			} else {
				insert_db_element_data(_db, data, _key, _value, translate_time_to_key(_ttl));
			}
			result.status = TABLE_UPDATED;
		}
	}
	return result;
}

req_status Delete_element(data_base* _db, const char* _name, const void* _key) {
	req_status result;
	db_element* curr_data = NULL;
	p_data curr_ptrs;
	result.status = UNDEFINED;
	if(_db != NULL && _name != NULL && _key != NULL) {
		result.status = TABLE_NOT_EXISTS;
		curr_data = find_db_table(_db, _name);
		if(curr_data != NULL) {
			result.status = TABLE_ELEMENT_NOT_EXISTS;
			curr_ptrs = find_ht_element(curr_data->table, _key);
			if(curr_ptrs.current != NULL) {
				result.status = TABLE_ELEMENT_FOUND;
				strcpy(result.buffer, curr_ptrs.current->table->get_element_value(curr_ptrs.current));
				delete_db_element_data(_db, curr_ptrs.current);
				result.status = TABLE_ELEMENT_REMOVED;
			}
		}
	}
	return result;
}

req_status Get_value(data_base* _db, const char* _name, const void* _key) {
	req_status result;
	db_element* curr_data = NULL;
	p_data curr_ptrs;
	result.status = UNDEFINED;
	if(_db != NULL && _name != NULL && _key != NULL) {
		result.status = TABLE_NOT_EXISTS;
		curr_data = find_db_table(_db, _name);
		if(curr_data != NULL){
			result.status = TABLE_ELEMENT_NOT_EXISTS;
			curr_ptrs = find_ht_element(curr_data->table, _key);
			if(curr_ptrs.current != NULL) {
				result.status = TABLE_ELEMENT_FOUND;
				strcpy(result.buffer, curr_ptrs.current->table->get_element_value(curr_ptrs.current));
			}
		}
	}
	return result;
}

/*..................................update base functions...............................................*/
void Update_base(data_base* _db) {
	if(_db != NULL) {
		update_binary_tree(_db->binary_tree);
	}
}

/*................................some static functions.................................................*/
static data_base* init_data_base() {
	data_base* new_base = NULL;
	new_base = (data_base*)malloc(sizeof(data_base));
	if(new_base != NULL) {
		new_base->data_head = NULL;
	}
	return new_base;
}

static db_element* init_db_element() {
	db_element* new_element = NULL;
	new_element = (db_element*)malloc(sizeof(db_element));
	if(new_element != NULL) {
		new_element->table = NULL;
		new_element->head = NULL;
		new_element->next = NULL;
		new_element->prev = NULL;
	}
	return new_element;
}

static void deinit_db_element(db_element* _element) {
	if(_element != NULL) {
		free(_element);
	}
}

static void remove_db_element(db_element* _element) {
	if(_element != NULL) {
		if (*(_element->head) == _element) { // head of list
			*(_element->head) = _element->next;
		}
		if(_element->prev != NULL) {
			_element->prev->next = _element->next;
		}
		if(_element->next != NULL) {
			_element->next->prev = _element->prev;
		}
		//delete_h_table(_element->table);
		deinit_db_element(_element);
	}
}

static db_element* find_db_table(data_base* _db, const char* _name) {
	db_element* curr_element = NULL;
	db_element* tmp_element = NULL;
	if(_db != NULL && _name != NULL) {
		tmp_element = _db->data_head;
		while(tmp_element != NULL) {
			if(strcmp(_name, tmp_element->table->name) == 0) { 
				curr_element = tmp_element;
				break;
			}
			tmp_element = tmp_element->next;
		}
	}
	return curr_element;
}

static void remove_db_table(data_base* _db, db_element* _element) {
	ht_element* curr_element = NULL;
	bt_node* curr_node = NULL;
	if (_db != NULL && _element != NULL) {
		curr_element = find_first_element(_element->table);
		while(curr_element != NULL) {
			if(curr_element->l_time != 0) {
				curr_node = find_bt_node(_db->binary_tree, curr_element->l_time);
				kill_bt_data(find_bt_data_by_node(curr_node, curr_element));
				if(is_empty_node(curr_node)){
					kill_bt_node(curr_node);
				}
			}
			delete_ht_element(_element->table, curr_element);
			curr_element = find_first_element(_element->table);
		}
		delete_h_table(_element->table);
		remove_db_element(_element);
	}
}

static void insert_db_element_data(data_base* _db, db_element* _dest, const void* _key, const void* _value, unsigned long long int _ttl) {
	ht_element* new_element = NULL;
	if (_db != NULL && _dest != NULL && _key != NULL && _value != NULL) {
		new_element = create_ht_element(_dest->table, _key, _value, _ttl);
		insert_ht_element(_dest->table, new_element); // hash-table element reg
		if(_ttl != 0) {
			insert_bt_data(insert_bt_node(_db->binary_tree, _ttl), new_element); // binary-tree element reg
		}
	}
}

static void delete_db_element_data(data_base* _db, ht_element* _element) {
	bt_node* node = NULL;
	if(_db != NULL && _element != NULL) {
		if(_element->l_time != 0) {
			node = find_bt_node(_db->binary_tree, _element->l_time);
			kill_bt_data(find_bt_data_by_node(node, _element));
			if(is_empty_node(node)){
				kill_bt_node(node);
			}
		}
		delete_ht_element(_element->table, _element);
	}
}

static void update_db_element_data(data_base* _db, ht_element* _old_element, const void* _key, const void* _value, unsigned long long int _ttl) {
	ht_element* new_element = NULL;
	bt_node* node = NULL;
	if(_db != NULL && _old_element != NULL && _key != NULL && _value != NULL) {
		new_element = create_ht_element(_old_element->table, _key, _value, _ttl);
		node = find_bt_node(_db->binary_tree, _ttl);
		kill_bt_data(find_bt_data_by_node(node, _old_element));
		if(is_empty_node(node)){
			kill_bt_node(node);
		}
		swap_ht_element(_old_element, new_element);
		kill_ht_element(_old_element);
		if(_ttl != 0) {
			insert_bt_data(insert_bt_node(_db->binary_tree, _ttl), new_element); // binary-tree element reg
		}
	}
}

static void update_binary_tree(b_tree* _tree) {
	bt_node* node = NULL;
	if(_tree != NULL) {
		node = find_bt_expired_node(_tree, get_current_time_value());
		delete_all_data_from_bt_node(node);
		kill_bt_node(node);
	}
}

static void delete_all_data_from_bt_node(bt_node* _node) {
	bt_data* element = NULL;
	if(_node != NULL) {
		element = get_bt_data_from_node(_node);
		while(element != NULL) {
			delete_ht_element(element->data->table, element->data);
			kill_bt_data(element);
			element = get_bt_data_from_node(_node);
		}
	}
}

static unsigned long long int get_current_time_value() {
 unsigned long long int time_value = 0;
 time_value = time(NULL);
 return time_value;
}

static unsigned long long int translate_time_to_key(unsigned long long int _time) {
	unsigned long long int key = 0;
	if(_time != 0) {
		key = (unsigned long long int)time(NULL) + _time;
	}
	return key;
}
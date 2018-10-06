
#include "db_handler.h"

data_base* Create_data_base() {
	data_base* new_base = NULL;
	new_base = init_data_base();
	if (new_base != NULL) {
		new_base->binary_tree = create_tree(&compare_uu, &compare_elements_hess);
	}
	return new_base;
}


void Create_table(data_base* _db, const char* _name) {
	db_element* new_element = NULL;
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
		}
	}
}

void Delete_table(data_base* _db, const char* _name) {
	db_element* curr_element = NULL;
	if(_db != NULL && _name != NULL) {
		curr_element = find_db_table(_db, _name);
		if (curr_element != NULL) {
			remove_db_table(_db, curr_element);
		}
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
			}
			delete_ht_element(_element->table, curr_element);
			curr_element = find_first_element(_element->table);
		}
		delete_h_table(_element->table);
		remove_db_element(_element);
	}
}
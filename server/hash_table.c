#include <stdlib.h>

#include "hash_table.h"


p_data find_ht_element(h_table* _tb, const void* _key) {
	p_data ptrs;
	ht_element* curr = NULL;
	int pos;
	ptrs.current = NULL;
	ptrs.head = NULL;
	if (_key != NULL && _tb != NULL) {
		pos = _tb->hash(_key) % HT_SIZE;
		ptrs.head = (_tb->data + pos);
		curr = *(_tb->data+pos);
		while(curr != NULL) {
			if(_tb->compare(_key, curr->key)) {
				ptrs.current = curr;
				break;
			}
			curr = curr->next;
		}
	}
	return ptrs;
}

h_table* create_h_table(const char* _name, cmp_f* _cmp, hash_f* _hash, get_f* _get, mem_init_f* _init) {
	h_table* new_table = NULL;
	if (_name != NULL && _cmp != NULL && _hash != NULL && _get != NULL && _init != NULL) {
		new_table = (h_table*)malloc(sizeof(h_table));
		if(new_table != NULL) {
			new_table->data = NULL;
			new_table->data = (ht_element**)malloc(sizeof(ht_element*) * HT_SIZE);
			if(new_table->data != NULL) {
				new_table->name = NULL;
				new_table->name = (char*)malloc(strlen(_name) + 1);
				if (new_table->name != NULL){
					strcpy(new_table->name, _name);
					new_table->compare = _cmp;
					new_table->hash = _hash;
					new_table->get_element_value = _get;
					new_table->create_element = _init;
					for(int i = 0; i < HT_SIZE; i++) {
						*(new_table->data+i) = NULL;
					}
				}
			}
		}
	}
	return new_table;
}

static void deinit_table(h_table* _table) {
	if(_table != NULL) {
		free(_table->name);
		free(_table->data);
		free(_table);
	}
}

void delete_h_table(h_table* _table) {
	if(_table != NULL) {
		for(int i = 0; i < HT_SIZE; i++) {
			remove_element_list(*(_table->data+i));
		}
		deinit_table(_table);
	}
}

ht_element* create_ht_element(h_table* _table, const void* _key, const void* _value, unsigned long long int _ttl) {
	ht_element* new_element = NULL;
	if (_table != NULL && _key != NULL && _value != NULL) {
		new_element = init_element();
		if(new_element != NULL) {
			_table->create_element(new_element, _key, _value);
			new_element->l_time = _ttl;
		}
	}
	return new_element;
}

static ht_element* init_element() {
	ht_element* new_element = NULL;
	new_element = (ht_element*)malloc(sizeof(ht_element));
	if(new_element != NULL) {
		new_element->table = NULL;
		new_element->key = NULL;
		new_element->value = NULL;
		new_element->head = NULL;
		new_element->next = NULL;
		new_element->prev = NULL;
		new_element->l_time = 0;
	}
	return new_element;
}

static void deinit_element(ht_element* _element) {
	if(_element != NULL) {
		free(_element);
	}
}

static void remove_element(ht_element* _element) {
	if(_element != NULL) {
		free(_element->value);
		free(_element->key);
		deinit_element(_element);
	}
}

static void remove_element_list(ht_element* _element) {
	if(_element != NULL) {
		remove_element_list(_element->next);
		remove_element(_element);
	}
}

void insert_ht_element(h_table* _table, ht_element* _element) {
	ht_element** head = NULL;
	int pos = 0;
	if(_table != NULL && _element != NULL) {
		_element->table = _table;
		pos = _table->hash(_element->key) % HT_SIZE;
		head = (_table->data+pos);
		if(*head == NULL) { // list is empty
			_element->head = head;
			*head = _element; // #atmf#
		} else {
			_element->head = head;
			_element->next = *head;
			_element->next->prev = _element; // #atmf#
			*head = _element; // #atmf#
		}
	}
}

ht_element* find_first_element(h_table* _tb) {
	ht_element* curr_element = NULL;
	if(_tb != NULL) {
		for(int i = 0; i < HT_SIZE; i++) {
			if(*(_tb->data+i) != NULL) {
				curr_element = *(_tb->data+i);
			}
		}
	}
	return curr_element;
}

void delete_ht_element(h_table* _tb, ht_element* _element) {
	if(_tb != NULL && _element != NULL) {
		if(*(_element->head) == _element) { // head of list
			*(_element->head) = _element->next;
		}
		if(_element->prev != NULL) {
			_element->prev->next = _element->next;
		}
		if(_element->next != NULL) {
			_element->next->prev = _element->prev;
		}
		remove_element(_element);
	}
}

void swap_ht_element(ht_element* _old, ht_element* _new) {
	if(_old != NULL && _new != NULL) {
		_new->table = _old->table;
		_new->head = _old->head;
		_new->prev = _old->prev;
		_new->next = _old->next;
		if(*(_old->head) == _old) { // head of the list
			*(_old->head) = _new;
		}
		if(_new->prev != NULL) {
			_new->prev->next = _new;
		}
		if(_new->next != NULL) {
			_new->next->prev = _new;
		}
	}
}

void kill_ht_element(ht_element* _element) {
	if(_element != NULL) {
		remove_element(_element);
	}
}

/*..................................hash/compare functions......................................*/
unsigned long long int hash_str(const void* _key) {
	unsigned long long int result = 0;
	size_t i;
	const char* str = NULL;

	if(_key != NULL){

		str = (const char*)_key;

		for(i = 0; i < strlen(str);i++) {
			result = result + (int)pow((int)(*(str+i)),i);
		}
	}

	return result;
}

int compare_ss(const void* _str1, const void* _str2) {
	int result = 0;

	if(_str1 != NULL && _str2 != NULL) {
		result = (strcmp((const char*)_str1,(const char*)_str2) == 0);
	}

	return result;
}

char* get_element_value_s(ht_element* _element) {
	char* val = NULL;
	if (_element != NULL) {
		if (_element->value != NULL) {
			val = (char*)_element->value;
		}
	}
	return val;
}

void create_element_ss(ht_element* _element, const void* _key, const void* _value) {
	size_t str_len = 0;
	if (_element != NULL && _key != NULL && _value != NULL) {
		str_len = strlen(_key);
		_element->key = malloc(str_len + 1);
		if (_element->key != NULL) {
			memcpy(_element->key, _key, str_len + 1);
		}
		str_len = strlen(_value);
		_element->value = malloc(str_len + 1);
		if (_element->value != NULL) {
			memcpy(_element->value, _value, str_len + 1);
		}
	}
}
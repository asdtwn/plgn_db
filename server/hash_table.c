#include <stdlib.h>

#include "hash_table.h"


p_data find_element(h_table* _tb, const void* _key) {
	p_data ptrs;
	ht_element* curr = NULL;
	int pos;
	ptrs.current = NULL;
	ptrs.head = NULL;
	if (_key != NULL && _tb != NULL) {
		pos = _tb->hash(_key) % HT_SIZE;
		ptrs.head = (_tb->data + pos);
		if (*(_tb->data + pos) == NULL) {
			ptrs.current = NULL;
		} else {
			curr = *(_tb->data+pos);
			while(curr->next != NULL) {
				if(_tb->compare(_key, curr->key)) {
					ptrs.current = curr;
					break;
				}
				curr = curr->next;
			}
		}
	}
	return ptrs;
}

h_table* create_table(const char* _name, cmp_f* _cmp, hash_f* _hash, mem_f* _mem, get_f* _get) {
	h_table* new_table = NULL;
	if (_name != NULL && _cmp != NULL && _hash != NULL && _mem != NULL && _get != NULL) {
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
					new_table->kill_element = _mem;
					new_table->get_element_value = _get;
					for(int i = 0; i < HT_SIZE; i++) {
						*(new_table->data+i) = NULL;
					}
				}
			}
		}
	}
	return new_table;
}

static ht_element* create_element(h_table* _table, const void* _key, const void* _value, unsigned long long int _time) {
	ht_element* new_element = NULL;
	if (_table != NULL && _key != NULL && _value != NULL) {
		new_element = init_element();
		if(new_element != NULL) {
			_table->create_element(new_element, _key, _value);
			new_element->l_time = _time;
			new_element->table = _table;
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

/*..................................hash/compare functions......................................*/
unsigned long long int hash_str(const void* _key) {
	unsigned long long int result = 0;
	size_t i;
	const char* str = NULL;

	if(_key != NULL){

		str = (const char*)_key;

		for(i = 0; ((*str) != '\0') && (i < MAX_STRING_LENGTH);i++) {
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

void kill_element_ss(ht_element* _element) {
	if(_element != NULL){
		if(*(_element->head) == _element) { // head of list
			if(_element->next != NULL) {
				_element->next->prev = NULL;
			}
			*(_element->head) = _element->next;
		} else {
			if(_element->prev != NULL) {
				_element->prev->next = _element->next;
			}
			if(_element->next != NULL) {
				_element->next->prev = _element->prev;
			}
		}
		// free memory
	}
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
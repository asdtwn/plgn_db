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

h_table* create_table(const char* _name, cmp_f* _cmp, hash_f* _hash) {
	h_table* new_table = NULL;
	if (_name != NULL && _cmp != NULL && _hash != NULL) {
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
					for(int i = 0; i < HT_SIZE; i++) {
						*(new_table->data+i) = NULL;
					}
				}
			}
		}
	}
	return new_table;
}

static ht_element* init_element() {
	ht_element* new_element = NULL;
	new_element = (ht_element*)malloc(sizeof(ht_element));
	if(new_element != NULL) {
		new_element->key = NULL;
		new_element->value = NULL;
		new_element->head = NULL;
		new_element->next = NULL;
		new_element->prev = NULL;
		new_element->l_time = 0;
	}
	return new_element;
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
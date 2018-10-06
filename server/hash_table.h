#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <math.h>
#include <string.h>

#define HT_SIZE 99
#define MAX_STRING_LENGTH 20

typedef int (cmp_f)(const void*, const void*);
typedef unsigned long long int (hash_f)(const void*);

typedef struct t_element{

	void* key;
	void* value;
	unsigned long long int l_time;

	struct t_element** head;
	struct t_element* next;
	struct t_element* prev;
} ht_element;


typedef struct hash_table{
	char* name;

	ht_element** data;

	cmp_f* compare;
	hash_f* hash;
} h_table;

typedef struct {
	ht_element* current;
	ht_element** head;
} p_data;


h_table* create_table(const char* _name, cmp_f* _cmp, hash_f* _hash);

ht_element* create_element(h_table* _table, const void* _key, const void* value, unsigned long long int _time);
void remove_element(ht_element* _element);
static ht_element* init_element();
static void deinit_element(ht_element* _element);
p_data find_element(h_table* _tb, const void* _key);

int compare_ss(const void* _str1, const void* _str2);
unsigned long long int hash_str(const void* _key);


#endif
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <math.h>
#include <string.h>
#include <stdio.h>

#define HT_SIZE 99

struct hash_table;
struct t_element;

typedef int (cmp_f)(const void*, const void*);
typedef unsigned long long int (hash_f)(const void*);
typedef char* (get_f)(struct t_element*);
typedef void (mem_init_f)(struct t_element*,const void*,const void*);

typedef struct t_element{
	struct hash_table* table;
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
	mem_init_f* create_element;
	get_f* get_element_value;
} h_table;

typedef struct {
	ht_element* current;
	ht_element** head;
} p_data;


h_table* create_h_table(const char* _name, cmp_f* _cmp, hash_f* _hash, get_f* _get, mem_init_f* _init);
void delete_h_table(h_table* _table); // recursively (not for table with TTL)

ht_element* create_ht_element(h_table* _table, const void* _key, const void* _value, unsigned long long int _ttl);
static void deinit_table(h_table* _table);
static void remove_element(ht_element* _element);
void kill_ht_element(ht_element* _element);
void delete_ht_element(h_table* _tb, ht_element* _element);
static void remove_element_list(ht_element* _element);
static ht_element* init_element();
static void deinit_element(ht_element* _element);
p_data find_ht_element(h_table* _tb, const void* _key);
void insert_ht_element(h_table* _table, ht_element* _element);
ht_element* find_first_element(h_table* _tb);
void swap_ht_element(ht_element* _old, ht_element* _new);

/*..................................hash/compare functions......................................*/
int compare_ss(const void* _str1, const void* _str2);
unsigned long long int hash_str(const void* _key);
char* get_element_value_s(ht_element* _element);
void create_element_ss(ht_element* _element, const void* _key, const void* _value);

#endif
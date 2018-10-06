#ifndef DB_HANDLER_H
#define DB_HANDLER_H


#include "hash_table.h"
#include "binary_tree.h"

typedef struct db_list_element {
	h_table* table;

	struct db_list_element** head;
	struct db_list_element* next;
	struct db_list_element* prev;
} db_element;

typedef struct db_data {
	db_element* data_head;
	b_tree* binary_tree;
} data_base;

/*...................................main interface functions...........................................*/
data_base* Create_data_base();
void Create_table(data_base* _db, const char* _name);
void Delete_table(data_base* _db, const char* _name);
void Update_table(data_base* _db, const char* _name, const void* _key, const void* _value, unsigned long long int _ttl);
void Remove_element(data_base* _db, const char* _name, const void* _key);
char* Get_value(data_base* _db, const char* _name, const void* _key);

/*................................some static functions.................................................*/
static data_base* init_data_base();
static db_element* init_db_element();
static void deinit_db_element();
static void remove_db_element();
static db_element* find_db_table(data_base* _db, const char* _name);
static void remove_db_table(data_base* _db, db_element* _element);

#endif
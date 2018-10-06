#ifndef DB_HANDLER_H
#define DB_HANDLER_H


#include "hash_table.h"
#include "binary_tree.h"

typedef enum db_operation_status {
	TABLE_CREATED,
	TABLE_REMOVED,
	TABLE_UPDATED,
	TABLE_ELEMENT_REMOVED,
	TABLE_ELEMENT_FOUND,
	TABLE_ELEMENT_NOT_EXISTS,
	TABLE_FOUND,
	TABLE_NOT_EXISTS,
	OK,
	UNDEFINED,
	ERROR
} f_status;

typedef struct db_request_status {
	f_status status;
	char buffer[2048];
} req_status;

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
req_status Create_table(data_base* _db, const char* _name);
req_status Delete_table(data_base* _db, const char* _name);
req_status Update_table(data_base* _db, const char* _name, const void* _key, const void* _value, unsigned long long int _ttl);
req_status Delete_element(data_base* _db, const char* _name, const void* _key);
req_status Get_value(data_base* _db, const char* _name, const void* _key);

/*................................some static functions.................................................*/
static data_base* init_data_base();
static db_element* init_db_element();
static void deinit_db_element();
static void remove_db_element();
static db_element* find_db_table(data_base* _db, const char* _name);
static void remove_db_table(data_base* _db, db_element* _element);
static void insert_db_element_data(data_base* _db, db_element* _dest, const void* _key, const void* _value, unsigned long long int _ttl);
static void update_db_element_data(data_base* _db, ht_element* _old_element, const void* _key, const void* _value, unsigned long long int _ttl);
static void delete_db_element_data(data_base* _db, ht_element* _element);

#endif
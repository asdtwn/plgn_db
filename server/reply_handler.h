#ifndef REPLY_HANDLER_H
#define REPLY_HANDLER_H

#define C_ADDRESS ("tcp://*:4040")

#include <czmq.h>
#include "db_handler.h"

typedef void (upd_f)(data_base*);

typedef struct request_data {
	int command;
	char* table_name;
	char* key;
	char* value;
	unsigned long long int ttl;
} r_data;

typedef struct zmq_data {
	void* context;
	void* socket;
} z_data;

void Init_connection(z_data* _data);
void Deinit_connection(z_data* _data);

void Start_receive(z_data* _data, data_base* _base, upd_f* _update);


/*..........................some static functions..........................*/
static void init_request_data(r_data* _data);
static void clear_request_data(r_data* _data);

static void handle_reply(r_data* _data, data_base* _base, void* _socket);
static void handle_create_table_reply(req_status* _data, void* _socket);
static void handle_delete_table_reply(req_status* _data, void* _socket);
static void handle_update_table_reply(req_status* _data, void* _socket);
static void handle_delete_t_element_reply(req_status* _data, void* _socket);
static void handle_get_t_element_reply(req_status* _data, void* _socket);


#endif
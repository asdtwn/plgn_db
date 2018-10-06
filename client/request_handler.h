#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#define C_ADDRESS ("tcp://localhost:4040")

#include <czmq.h>

typedef enum reply_status {
	TABLE_CREATED, // 0
	TABLE_REMOVED, // 1
	TABLE_UPDATED, // 2
	TABLE_ELEMENT_REMOVED, // 3
	TABLE_ELEMENT_FOUND, // 4
	TABLE_ELEMENT_NOT_EXISTS, // 5
	TABLE_FOUND, // 6
	TABLE_NOT_EXISTS, // 7
	OK, // 8
	UNDEFINED, // 9
	ERROR // 10
} r_status;

typedef struct rep_data {
	r_status status;
	char buffer[2048];
} reply;

typedef struct zmq_data {
	void* context;
	void* socket;
} z_data;

void Create_table(z_data* _data, const char* _name); // 0
void Delete_table(z_data* _data, const char* _name); // 1
void Update_table(z_data* _data, const char* _name, const char* _key, const char* _value, unsigned long long int _ttl); // 2
void Delete_element(z_data* _data, const char* _name, const char* _key); // 3
void Get_element(z_data* _data, const char* _name, const char* _key); // 4

void Init_connection(z_data* _data);
void Deinit_connection(z_data* _data);
static reply send_request(z_data* _data, unsigned char _com_id, const char* _name, const char* _key, const char* _value, const unsigned long long int* _ttl);
static reply receive_reply(z_data* _data);



















#endif
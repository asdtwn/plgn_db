#include "reply_handler.h"

void Init_connection(z_data* _data) {
	if(_data != NULL) {
		_data->context = zmq_ctx_new();
		_data->socket = zmq_socket(_data->context, ZMQ_REP);
		zmq_bind(_data->socket, C_ADDRESS);
	}
}

void Deinit_connection(z_data* _data) {
	if(_data != NULL) {
		zmq_close(_data->socket);
		zmq_ctx_destroy(_data->context);
	}
}

void Start_receive(z_data* _data, data_base* _base, upd_f* _update) {
	zmq_msg_t msg_part;
	unsigned long long int more;
	int part_counter;
	size_t size = sizeof(more);
	r_data curr_request;
	void* buffer[1024];
	init_request_data(&curr_request);
	if(_data != NULL && _base != NULL && _update != NULL) {
		while(1) {
			_update(_base); // TMP data_base UPDATE !!!!! (blocked by receive)
			more = 1;
			part_counter = 1;
			while(more) { // handle current request
				zmq_msg_init(&msg_part);
				zmq_msg_recv(&msg_part, _data->socket, 0);
				zmq_getsockopt(_data->socket, ZMQ_RCVMORE, &more, &size);
				memcpy(buffer, zmq_msg_data(&msg_part), zmq_msg_size(&msg_part));
				*(char*)(buffer+zmq_msg_size(&msg_part)) = '\0';
				switch (part_counter) {

					case 1: // command
					curr_request.command = *(unsigned char*)buffer;
					break;

					case 2: // table name
					curr_request.table_name = malloc(strlen((char*)buffer) + 1);
					memcpy(curr_request.table_name, buffer, strlen((char*)buffer) + 1);
					break;

					case 3: // key
					curr_request.key = malloc(strlen((char*)buffer) + 1);
					memcpy(curr_request.key, buffer, strlen((char*)buffer) + 1);
					break;

					case 4: // value
					curr_request.value = malloc(strlen((char*)buffer) + 1);
					memcpy(curr_request.value, buffer, strlen((char*)buffer) + 1);
					break;

					case 5: // ttl
					curr_request.ttl = *(unsigned long long int*)buffer;
					break;
				}
				zmq_msg_close(&msg_part);
				part_counter++;
			}
			handle_reply(&curr_request, _base, _data->socket); // SEND REPLY
			clear_request_data(&curr_request);
		}
	}
}

/*..........................some static functions..........................*/
static void init_request_data(r_data* _data) {
	if(_data != NULL) {
		_data->table_name = NULL;
		_data->key = NULL;
		_data->value = NULL;
		_data->ttl = 0;
	}
}

static void clear_request_data(r_data* _data) {
	if(_data != NULL) {
		free(_data->table_name);
		_data->table_name = NULL;
		free(_data->key);
		_data->key = NULL;
		free(_data->value);
		_data->value = NULL;
		_data->ttl = 0;
	}
}

static void handle_reply(r_data* _data, data_base* _base, void* _socket) {
	req_status request;
	if(_data != NULL && _base != NULL && _socket != NULL) {

		switch (_data->command) {

			case 0: // CREATE TABLE
			request = Create_table(_base, _data->table_name);
			handle_create_table_reply(&request, _socket);
			break;

			case 1: // DELETE TABLE
			request = Delete_table(_base, _data->table_name);
			handle_delete_table_reply(&request, _socket);
			break;

			case 2: // UPDATE TABLE
			request = Update_table(_base, _data->table_name, _data->key, _data->value, _data->ttl);
			handle_update_table_reply(&request, _socket);
			break;

			case 3: // DELETE ELEMENT
			request = Delete_element(_base, _data->table_name, _data->key);
			handle_delete_t_element_reply(&request, _socket);
			break;

			case 4: // GET VALUE
			request = Get_value(_base, _data->table_name, _data->key);
			handle_get_t_element_reply(&request, _socket);
			break;

		}
	}
}

static void handle_create_table_reply(req_status* _data, void* _socket) {
	zmq_msg_t req_status, error_reason;
	unsigned char res;
	char* reason;
	if(_data != NULL && _socket != NULL) {
		if(_data->status == TABLE_CREATED) { // OK
			res = 1;
			zmq_msg_init_size(&req_status, 1);
			memcpy(zmq_msg_data(&req_status), &res, 1);
			zmq_msg_send(&req_status, _socket, 0);
			zmq_msg_close(&req_status);
		} else { // ERROR
			res = 0;
			reason = "UNKNOWN ERROR";
			zmq_msg_init_size(&req_status, 1);
			memcpy(zmq_msg_data(&req_status), &res, 1);
			zmq_msg_send(&req_status, _socket, ZMQ_SNDMORE);
			zmq_msg_init_size(&error_reason, strlen(reason));
			memcpy(zmq_msg_data(&error_reason), reason, strlen(reason));
			zmq_msg_send(&error_reason, _socket, 0);
			zmq_msg_close(&req_status);
			zmq_msg_close(&error_reason);
		}
	}
}

static void handle_delete_table_reply(req_status* _data, void* _socket) {\
	zmq_msg_t req_status, error_reason;
	unsigned char res;
	char* reason;
	if(_data != NULL && _socket != NULL) {
		if(_data->status == TABLE_REMOVED) { // ok
			res = 1;
			zmq_msg_init_size(&req_status, 1);
			memcpy(zmq_msg_data(&req_status), &res, 1);
			zmq_msg_send(&req_status, _socket, 0);
			zmq_msg_close(&req_status);
		} else { // error
			res = 0;
			reason = "TABLE NOT FOUND";
			zmq_msg_init_size(&req_status, 1);
			memcpy(zmq_msg_data(&req_status), &res, 1);
			zmq_msg_send(&req_status, _socket, ZMQ_SNDMORE);
			zmq_msg_init_size(&error_reason, strlen(reason));
			memcpy(zmq_msg_data(&error_reason), reason, strlen(reason));
			zmq_msg_send(&error_reason, _socket, 0);
			zmq_msg_close(&req_status);
			zmq_msg_close(&error_reason);
		}
	}
}

static void handle_update_table_reply(req_status* _data, void* _socket) {
	zmq_msg_t req_status, error_reason;
	unsigned char res;
	char* reason;
	if(_data != NULL && _socket != NULL) {
		if(_data->status == TABLE_UPDATED) { // ok
			res = 1;
			zmq_msg_init_size(&req_status, 1);
			memcpy(zmq_msg_data(&req_status), &res, 1);
			zmq_msg_send(&req_status, _socket, 0);
			zmq_msg_close(&req_status);
		} else { // error
			res = 0;
			if(_data->status == TABLE_NOT_EXISTS) {
				reason = "TABLE NOT FOUND";
			} else {
				reason = "TABLE ELEMENT NOT FOUND";
			}
			zmq_msg_init_size(&req_status, 1);
			memcpy(zmq_msg_data(&req_status), &res, 1);
			zmq_msg_send(&req_status, _socket, ZMQ_SNDMORE);
			zmq_msg_init_size(&error_reason, strlen(reason));
			memcpy(zmq_msg_data(&error_reason), reason, strlen(reason));
			zmq_msg_send(&error_reason, _socket, 0);
			zmq_msg_close(&req_status);
			zmq_msg_close(&error_reason);
		}
	}
}

static void handle_delete_t_element_reply(req_status* _data, void* _socket) {
	zmq_msg_t req_status, error_reason, tb_value;
	unsigned char res;
	char* reason;
	if(_data != NULL && _socket != NULL) {
		if(_data->status == TABLE_ELEMENT_REMOVED) { // ok
			res = 1;
			zmq_msg_init_size(&req_status, 1);
			memcpy(zmq_msg_data(&req_status), &res, 1);
			zmq_msg_send(&req_status, _socket, ZMQ_SNDMORE);
			zmq_msg_init_size(&tb_value, strlen(_data->buffer));
			memcpy(zmq_msg_data(&tb_value), _data->buffer, strlen(_data->buffer));
			zmq_msg_send(&tb_value, _socket, 0);
			zmq_msg_close(&tb_value);
			zmq_msg_close(&req_status);
		} else { // error
			res = 0;
			if(_data->status == TABLE_NOT_EXISTS) {
				reason = "TABLE NOT FOUND";
			} else {
				reason = "TABLE ELEMENT NOT FOUND";
			}
			zmq_msg_init_size(&req_status, 1);
			memcpy(zmq_msg_data(&req_status), &res, 1);
			zmq_msg_send(&req_status, _socket, ZMQ_SNDMORE);
			zmq_msg_init_size(&error_reason, strlen(reason));
			memcpy(zmq_msg_data(&error_reason), reason, strlen(reason));
			zmq_msg_send(&error_reason, _socket, 0);
			zmq_msg_close(&req_status);
			zmq_msg_close(&error_reason);
		}
	}
}

static void handle_get_t_element_reply(req_status* _data, void* _socket) {
	zmq_msg_t req_status, error_reason, tb_value;
	unsigned char res;
	char* reason;
	if(_data != NULL && _socket != NULL) {
		if(_data->status == TABLE_ELEMENT_FOUND) { // ok
			res = 1;
			zmq_msg_init_size(&req_status, 1);
			memcpy(zmq_msg_data(&req_status), &res, 1);
			zmq_msg_send(&req_status, _socket, ZMQ_SNDMORE);
			zmq_msg_init_size(&tb_value, strlen(_data->buffer));
			memcpy(zmq_msg_data(&tb_value), _data->buffer, strlen(_data->buffer));
			zmq_msg_send(&tb_value, _socket, 0);
			zmq_msg_close(&tb_value);
			zmq_msg_close(&req_status);
		} else { // error
			res = 0;
			if(_data->status == TABLE_NOT_EXISTS) {
				reason = "TABLE NOT FOUND";
			} else {
				reason = "TABLE ELEMENT NOT FOUND";
			}
			zmq_msg_init_size(&req_status, 1);
			memcpy(zmq_msg_data(&req_status), &res, 1);
			zmq_msg_send(&req_status, _socket, ZMQ_SNDMORE);
			zmq_msg_init_size(&error_reason, strlen(reason));
			memcpy(zmq_msg_data(&error_reason), reason, strlen(reason));
			zmq_msg_send(&error_reason, _socket, 0);
			zmq_msg_close(&req_status);
			zmq_msg_close(&error_reason);
		}
	}
}
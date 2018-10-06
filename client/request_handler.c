#include "request_handler.h"

void Init_connection(z_data* _zmq_data) {
	if(_zmq_data != NULL) {
		_zmq_data->context = zmq_ctx_new();
		_zmq_data->socket = zmq_socket(_zmq_data->context, ZMQ_REQ);
		zmq_connect(_zmq_data->socket, C_ADDRESS);
	}
}

void Deinit_connection(z_data* _data) {
	if(_data != NULL) {
		zmq_close(_data->socket);
		zmq_ctx_destroy(_data->context);
	}
}

void Create_table(z_data* _data, const char* _name) {
		unsigned char com_id = 0;
		reply srv_rep;
	if(_data != NULL && _name != NULL) {
		send_request(_data, &srv_rep, com_id, _name, NULL, NULL, NULL);
		if(srv_rep.status == OK) {
			printf("status - OK::Table created\n");
		} else {
			printf("status - ERROR::%s\n", srv_rep.buffer);
		}
	}
}

void Delete_table(z_data* _data, const char* _name) {
	unsigned char com_id = 1;
	reply srv_rep;
	if(_data != NULL && _name != NULL) {
		send_request(_data, &srv_rep, com_id, _name, NULL, NULL, NULL);
		if(srv_rep.status == OK) {
			printf("status - OK::Table deleted\n");
		} else {
			printf("status - ERROR::%s\n", srv_rep.buffer);
		}
	}
}

void Update_table(z_data* _data, const char* _name, const char* _key, const char* _value, unsigned long long int _ttl) {
	unsigned char com_id = 2;
	reply srv_rep;
	if(_data != NULL && _name != NULL && _key != NULL && _value != NULL) {
		send_request(_data, &srv_rep, com_id, _name, _key, _value, &_ttl);
		if(srv_rep.status == OK) {
			printf("status - OK::Table updated\n");
		} else {
			printf("status - ERROR::%s\n", srv_rep.buffer);
		}
	}
}

void Delete_element(z_data* _data, const char* _name, const char* _key) {
	unsigned char com_id = 3;
	reply srv_rep;
	if(_data != NULL && _name != NULL && _key != NULL) {
		send_request(_data, &srv_rep, com_id, _name, _key, NULL, NULL);
		if(srv_rep.status == OK) {
			printf("status - OK::%s\n", srv_rep.buffer);
		} else {
			printf("status - ERROR::%s\n", srv_rep.buffer);
		}
	}
}

void Get_element(z_data* _data, const char* _name, const char* _key) {
	unsigned char com_id = 4;
	reply srv_rep;
	if(_data != NULL && _name != NULL && _key != NULL) {
		send_request(_data, &srv_rep, com_id, _name, _key, NULL, NULL);
		if(srv_rep.status == OK) {
			printf("status - OK::%s\n", srv_rep.buffer);
		} else {
			printf("status - ERROR::%s\n", srv_rep.buffer);
		}
	}
}

static void send_request(z_data* _data, reply* server_reply, unsigned char _com_id, const char* _name, const char* _key, const char* _value, const unsigned long long int* _ttl) {
	zmq_msg_t command, name, key, value, ttl;
	if(_data != NULL) {
		switch (_com_id) {
			/*.........................create table.........................*/
			case 0:
			if(_name != NULL) {
				zmq_msg_init_size(&command, 1);
				zmq_msg_init_size(&name, strlen(_name));
				memcpy(zmq_msg_data(&command), &_com_id, 1);
				memcpy(zmq_msg_data(&name), _name, strlen(_name));
				zmq_msg_send(&command, _data->socket, ZMQ_SNDMORE);
				zmq_msg_send(&name, _data->socket, 0);
				zmq_msg_close(&command);
				zmq_msg_close(&name);
				receive_reply(_data, server_reply);
			}
			break;

			/*.........................delete table.........................*/
			case 1:
			if(_name != NULL) {
				zmq_msg_init_size(&command, 1);
				zmq_msg_init_size(&name, strlen(_name));
				memcpy(zmq_msg_data(&command), &_com_id, 1);
				memcpy(zmq_msg_data(&name), _name, strlen(_name));
				zmq_msg_send(&command, _data->socket, ZMQ_SNDMORE);
				zmq_msg_send(&name, _data->socket, 0);
				zmq_msg_close(&command);
				zmq_msg_close(&name);
				receive_reply(_data, server_reply);
			}
			break;

			/*.........................update table.........................*/
			case 2:
			if(_name != NULL && _key != NULL && _value != NULL) {
				zmq_msg_init_size(&command, 1);
				zmq_msg_init_size(&name, strlen(_name));
				zmq_msg_init_size(&key, strlen(_key));
				zmq_msg_init_size(&value, strlen(_value));
				memcpy(zmq_msg_data(&command), &_com_id, 1);
				memcpy(zmq_msg_data(&name), _name, strlen(_name));
				memcpy(zmq_msg_data(&key), _key, strlen(_key));
				memcpy(zmq_msg_data(&value), _value, strlen(_value));
				zmq_msg_send(&command, _data->socket, ZMQ_SNDMORE);
				zmq_msg_send(&name, _data->socket, ZMQ_SNDMORE);
				zmq_msg_send(&key, _data->socket, ZMQ_SNDMORE);
				if(*(_ttl) != 0) {
					zmq_msg_send(&value, _data->socket, ZMQ_SNDMORE);
					zmq_msg_init_size(&ttl, sizeof(_ttl));
					memcpy(zmq_msg_data(&ttl), _ttl, sizeof(_ttl));
					zmq_msg_send(&ttl, _data->socket, 0);
					zmq_msg_close(&ttl);
				} else {
					zmq_msg_send(&value, _data->socket, 0);
				}
				zmq_msg_close(&command);
				zmq_msg_close(&name);
				zmq_msg_close(&key);
				zmq_msg_close(&value);
				receive_reply(_data, server_reply);
			}
			break;

			/*.........................delete element.........................*/
			case 3:
			if(_name != NULL && _key != NULL) {
				zmq_msg_init_size(&command, 1);
				zmq_msg_init_size(&name, strlen(_name));
				zmq_msg_init_size(&key, strlen(_key));
				memcpy(zmq_msg_data(&command), &_com_id, 1);
				memcpy(zmq_msg_data(&name), _name, strlen(_name));
				memcpy(zmq_msg_data(&key), _key, strlen(_key));
				zmq_msg_send(&command, _data->socket, ZMQ_SNDMORE);
				zmq_msg_send(&name, _data->socket, ZMQ_SNDMORE);
				zmq_msg_send(&key, _data->socket, 0);
				zmq_msg_close(&command);
				zmq_msg_close(&name);
				zmq_msg_close(&key);
				receive_reply(_data, server_reply);
			}
			break;

			/*.........................get element.........................*/
			case 4:
			if(_name != NULL && _key != NULL) {
				zmq_msg_init_size(&command, 1);
				zmq_msg_init_size(&name, strlen(_name));
				zmq_msg_init_size(&key, strlen(_key));
				memcpy(zmq_msg_data(&command), &_com_id, 1);
				memcpy(zmq_msg_data(&name), _name, strlen(_name));
				memcpy(zmq_msg_data(&key), _key, strlen(_key));
				zmq_msg_send(&command, _data->socket, ZMQ_SNDMORE);
				zmq_msg_send(&name, _data->socket, ZMQ_SNDMORE);
				zmq_msg_send(&key, _data->socket, 0);
				zmq_msg_close(&command);
				zmq_msg_close(&name);
				zmq_msg_close(&key);
				receive_reply(_data, server_reply);
			}
			break;
		}
	}
}

static void receive_reply(z_data* _data, reply* _rep) {
	zmq_msg_t msg_part;
	unsigned long long int more;
	int rep_stat;
	size_t more_size = sizeof(more);
	if(_data != NULL && _rep != NULL) {
		zmq_msg_init(&msg_part);
		zmq_msg_recv(&msg_part, _data->socket, 0);
		zmq_getsockopt(_data->socket, ZMQ_RCVMORE, &more, &more_size);
		rep_stat = *((unsigned char*)zmq_msg_data(&msg_part));
		if(rep_stat  == 1) { // OK
			_rep->status = OK;
			strcpy(_rep->buffer, "");
		} else { // ERROR
			_rep->status = ERROR;
			strcpy(_rep->buffer, "");
		}
		zmq_msg_close(&msg_part);
		if(more) {
			zmq_msg_init(&msg_part);
			zmq_msg_recv(&msg_part, _data->socket, 0);
			memcpy(_rep->buffer, zmq_msg_data(&msg_part), zmq_msg_size(&msg_part));
			_rep->buffer[zmq_msg_size(&msg_part)] = '\0';
			zmq_msg_close(&msg_part);
		}
	}
}
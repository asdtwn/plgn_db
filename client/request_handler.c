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
		srv_rep = send_request(_data, com_id, _name, NULL, NULL, NULL);
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
		srv_rep = send_request(_data, com_id, _name, NULL, NULL, NULL);
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
		srv_rep = send_request(_data, com_id, _name, _key, _value, &_ttl);
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
		srv_rep = send_request(_data, com_id, _name, _key, NULL, NULL);
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
		srv_rep = send_request(_data, com_id, _name, _key, NULL, NULL);
		if(srv_rep.status == OK) {
			printf("status - OK::%s\n", srv_rep.buffer);
		} else {
			printf("status - ERROR::%s\n", srv_rep.buffer);
		}
	}
}

static reply send_request(z_data* _data, unsigned char _com_id, const char* _name, const char* _key, const char* _value, const unsigned long long int* _ttl) {
	zmq_msg_t command, name, key, value, ttl;
	reply server_reply;
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
				server_reply = receive_reply(_data);
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
				server_reply = receive_reply(_data);
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
				server_reply = receive_reply(_data);
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
				server_reply = receive_reply(_data);
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
				server_reply = receive_reply(_data);
			}
			break;
		}
	}
	return server_reply;
}

static reply receive_reply(z_data* _data) {
	zmq_msg_t msg_part;
	reply status_reply;
	unsigned long long int more;
	size_t more_size = sizeof(more);
	if(_data != NULL) {
		zmq_msg_init(&msg_part);
		zmq_msg_recv(&msg_part, _data->socket, 0);
		zmq_getsockopt(_data->socket, ZMQ_RCVMORE, &more, &more_size);
		if(*((unsigned char*)zmq_msg_data(&msg_part))  == 0) { // OK
			status_reply.status = OK;
		} else { // ERROR
			status_reply.status = ERROR;
		}
		zmq_msg_close(&msg_part);
		if(more) {
			zmq_msg_init(&msg_part);
			zmq_msg_recv(&msg_part, _data->socket, 0);
			//strcpy(status_reply.buffer, zmq_msg_data(&msg_part));
			memcpy(status_reply.buffer, zmq_msg_data(&msg_part), zmq_msg_size(&msg_part));
			status_reply.buffer[zmq_msg_size(&msg_part)] = '\0';
			zmq_msg_close(&msg_part);
		}
	}
	return status_reply;
}
#include <stdio.h>
#include <stdlib.h>
#include "request_handler.h"


int main(int argc, char** argv) {
/*.....................args: COMMAND(0-4) <TABLE_NAME> <KEY> <VALUE> <TTL>..........*/

	unsigned long long int ttl = 0;
	if(argc >= 6){
		ttl = (unsigned long long int)atoi(*(argv+5));
	}
	if(argc >= 3) {
	z_data zmq_data;
	Init_connection(&zmq_data);
	switch (atoi((char*)(*(argv+1)))){
		case 0:
		Create_table(&zmq_data, (*(argv+2)));
		break;

		case 1:
		Delete_table(&zmq_data, (*(argv+2)));
		break;

		case 2:
		if(argc >= 5) {
			Update_table(&zmq_data, (*(argv+2)), (*(argv+3)), (*(argv+4)), ttl);
		}
		break;

		case 3:
		if(argc >= 4) {
			Delete_element(&zmq_data, (*(argv+2)), (*(argv+3)));
		}
		break;

		case 4:
		if(argc >= 4) {
			Get_element(&zmq_data, (*(argv+2)), (*(argv+3)));
		}
		break;

		default:
		printf("Wrong command (try 0-4)\n");
		break;
	}	

	Deinit_connection(&zmq_data);
	}
	return 0;
}
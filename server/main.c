#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <czmq.h>

#include "db_handler.h"
#include "reply_handler.h"

int main() {
	
	data_base* base = Create_data_base();
	z_data con_data;
	Init_connection(&con_data);
	Start_receive(&con_data, base, &Update_base);
	Deinit_connection(&con_data);












	// local tests
	/*
	unsigned long long int tm = 0;
	unsigned long long int start_tm = 0;
	req_status ptr_data; // status struct

	//.1............................create base............................./
	data_base* base = Create_data_base();

	//.2............................create table............................/
	Create_table(base, "Money");

	//.3............................insert new value......................../
	Update_table(base, "Money", "first", "million", 11);

	//.4............................update table value....................../
	Update_table(base, "Money", "first", "million", 50);

	//.4............................delete table value....................../
	Delete_element(base, "Money", "first");

	//.5............................get element............................./
	Delete_table(base, "Money");

	//.6............................get element............................./
	start_tm = time(NULL);
	while(1) {
		tm = time(NULL) - start_tm;
		Update_base(base);
		if (tm >= 10) {
			ptr_data = Get_value(base, "Money", "first");
			printf("status = %d \n", ptr_data.status);
			printf("string = %s \n", ptr_data.buffer);
			break;
		}
	}
	*/
	return 0;
}
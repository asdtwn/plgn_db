
 = SERVER =
gcc main.c hash_table.c binary_tree.c db_handler.c reply_handler.c -lzmq -lczmq -lm -o srv.exe

= CLIENT =
gcc main.c request_handler.c -lzmq -lczmq -o client.exe
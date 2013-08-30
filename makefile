all:client server
.PHONY:all

objects1 =  client_ls.h client_get.h client_sortget.h client_put.h client_watch.h 
objects2 =  server_ls.h server_get.h server_sortget.h server_put.h server_watch.h sys_lock.h err_lock.h

client:client_main.c
	gcc -o client client_main.c


server:server_main.c
	gcc -o server server_main.c


.PHONY:clean
clean:
	rm -f *.o

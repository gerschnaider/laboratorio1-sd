all: hijo_exec.out ejecutable_server_execv.out ejecutable_server_system.out

hijo_exec.out: hijo_exec.c
	gcc hijo_exec.c -o hijo_exec.out

ejecutable_server_execv.out: proto_svc.o proto_server_execv.o
	gcc -o ejecutable_server_execv.out proto_svc.o proto_server_execv.o -ltirpc

ejecutable_server_system.out: proto_svc.o proto_server_system.o
	gcc -o ejecutable_server_system.out proto_svc.o proto_server_system.o -ltirpc

proto_svc.o: proto_svc.c
	gcc -c proto_svc.c -I/usr/include/tirpc

proto_server_execv.o: proto_server_execv.c
	gcc -c proto_server_execv.c -I/usr/include/tirpc

proto_server_system.o: proto_server_system.c
	gcc -c proto_server_system.c -I/usr/include/tirpc

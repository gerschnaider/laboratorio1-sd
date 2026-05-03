all: clienteEjecutable.out

clienteEjecutable.out: proto_client.o proto_clnt.o
	gcc -o clienteEjecutable.out proto_client.o proto_clnt.o -ltirpc -I/usr/include/tirpc

proto_client.o: proto_client.c
	gcc -c proto_client.c -I/usr/include/tirpc

proto_clnt.o: proto_clnt.c
	gcc -c proto_clnt.c -I/usr/include/tirpc


# Regla principal que ejecuta todo
all: clienteEjecutable.out

clienteEjecutable.out: proto_client.o proto_clnt.o proto_xdr.o
	gcc -o clienteEjecutable.out proto_client.o proto_clnt.o proto_xdr.o -ltirpc -I/usr/include/tirpc

proto_client.o: proto_client.c
	gcc -c proto_client.c -I/usr/include/tirpc

proto_clnt.o: proto_clnt.c
	gcc -c proto_clnt.c -I/usr/include/tirpc

proto_xdr.o: proto_xdr.c
	gcc -c proto_xdr.c -I/usr/include/tirpc

# Regla opcional para limpiar la carpeta
clean:
	rm -f *.o clienteEjecutable.out
all: serverEjecutable.out

serverEjecutable.out: proto_server.o proto_svc.o proto_xdr.o
	gcc -o serverEjecutable.out proto_server.o proto_svc.o proto_xdr.o -ltirpc -I/usr/include/tirpc

proto_server.o: proto_server.c
	gcc -c proto_server.c -I/usr/include/tirpc

proto_svc.o: proto_svc.c
	gcc -c proto_svc.c -I/usr/include/tirpc

proto_xdr.o: proto_xdr.c
	gcc -c proto_xdr.c -I/usr/include/tirpc
	
# Regla opcional para limpiar la carpeta
clean:
	rm -f *.o serverEjecutable.out
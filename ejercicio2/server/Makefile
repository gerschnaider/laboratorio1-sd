# Regla principal que ejecuta todo
all: serverEjecutable.out

# 1. Armamos el ejecutable final juntando los .o
serverEjecutable.out: proto_server.o proto_svc.o proto_xdr.o
	gcc -o serverEjecutable.out proto_server.o proto_svc.o proto_xdr.o -ltirpc -I/usr/include/tirpc

# 2. Compilamos el código principal de tu servidor
proto_server.o: proto_server.c
	gcc -c proto_server.c -I/usr/include/tirpc

# 3. Compilamos el stub del servidor (generado por rpcgen)
proto_svc.o: proto_svc.c
	gcc -c proto_svc.c -I/usr/include/tirpc

# 4. Compilamos las rutinas de serialización XDR (generadas por rpcgen)
proto_xdr.o: proto_xdr.c
	gcc -c proto_xdr.c -I/usr/include/tirpc

# 5. Regla opcional para limpiar la carpeta
clean:
	rm -f *.o serverEjecutable.out
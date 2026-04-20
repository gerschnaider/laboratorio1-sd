# Regla principal que ejecuta todo
all: clienteEjecutable

# 1. Armamos el ejecutable final juntando los .o
clienteEjecutable: proto_client.o proto_clnt.o proto_xdr.o
	gcc -o clienteEjecutable proto_client.o proto_clnt.o proto_xdr.o -ltirpc -I/usr/include/tirpc

# 2. Compilamos el código principal de tu cliente
proto_client.o: proto_client.c
	gcc -c proto_client.c -I/usr/include/tirpc

# 3. Compilamos el stub del cliente (generado por rpcgen)
proto_clnt.o: proto_clnt.c
	gcc -c proto_clnt.c -I/usr/include/tirpc

# 4. Compilamos las rutinas de serialización XDR (generadas por rpcgen)
proto_xdr.o: proto_xdr.c
	gcc -c proto_xdr.c -I/usr/include/tirpc

# 5. Regla opcional para limpiar la carpeta
clean:
	rm -f *.o clienteEjecutable
all: ejecutable_queue_admin.out ejecutable_cliente.out ejecutable_servidor.out

ejecutable_queue_admin.out: queue_admin/main_queue_admin.c
	gcc queue_admin/main_queue_admin.c -o queue_admin/ejecutable_queue_admin.out -lpthread

ejecutable_cliente.out: producer/main_producer.c
	gcc producer/main_producer.c -o producer/ejecutable_cliente.out -lpthread

ejecutable_servidor.out: consumer/main_consumer.c
	gcc consumer/main_consumer.c -o consumer/ejecutable_servidor.out -lpthread
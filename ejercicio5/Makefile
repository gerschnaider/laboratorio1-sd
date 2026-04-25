all: ejecutable_queue_admin ejecutable_cliente ejecutable_servidor

ejecutable_queue_admin: queue_admin/main_queue_admin.c
	gcc queue_admin/main_queue_admin.c -o queue_admin/ejecutable_queue_admin -lpthread

ejecutable_cliente: producer/main_producer.c
	gcc producer/main_producer.c -o producer/ejecutable_cliente -lpthread

ejecutable_servidor: consumer/main_consumer.c
	gcc consumer/main_consumer.c -o consumer/ejecutable_servidor -lpthread
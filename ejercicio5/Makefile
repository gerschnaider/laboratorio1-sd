all: queue_admin/ejecutable_queue_admin.out producer/ejecutable_producer.out consumer/ejecutable_consumer.out

queue_admin/ejecutable_queue_admin.out: queue_admin/main_queue_admin.c
	gcc queue_admin/main_queue_admin.c -o queue_admin/ejecutable_queue_admin.out -lpthread

producer/ejecutable_producer.out: producer/main_producer.c
	gcc producer/main_producer.c -o producer/ejecutable_producer.out -lpthread

consumer/ejecutable_consumer.out: consumer/main_consumer.c
	gcc consumer/main_consumer.c -o consumer/ejecutable_consumer.out -lpthread
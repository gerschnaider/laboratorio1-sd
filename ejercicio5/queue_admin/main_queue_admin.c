#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>

// Estructura obligatoria para System V
typedef struct MensajeCola{
    long mtype;       // OBLIGATORIO: Debe ser el primer elemento y mayor a 0
    char texto[100];
} MensajeCola;

int msqid;

// Función para borrar la cola
void borrar_cola() {
    if (msqid != -1) {
        if (msgctl(msqid, IPC_RMID, NULL) == -1) {
            perror("Error al borrar la cola");
        } else {
            printf("\n[Sistema] Cola de mensajes (ID: %d) eliminada correctamente.\n", msqid);
        }
    }
}

// Manejador de señales para Ctrl+C
void handle_sigint(int sig) {
    borrar_cola();
    exit(0); // Sale del programa limpiamente
}



void inicializar_cola() {
    key_t clave = ftok(".", 'A');
    if (clave == -1) {
        perror("Error en ftok");
        exit(1);
    }

    // Intentamos obtener una cola que ya exista con esa clave
    int id_viejo = msgget(clave, 0666);
    if (id_viejo != -1) {
        printf("[Inicio] Se detectó una cola existente. Limpiando...\n");
        msgctl(id_viejo, IPC_RMID, NULL);
    }

    // Crear la cola nueva
    msqid = msgget(clave, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("Error en msgget");
        exit(1);
    }

    printf("Cola creada exitosamente con ID: %d\n", msqid);
}

void thread_handle_producers()
{
    // Inicio un socket UDP para escuchar a los productores
    int sockfd = socket(AF_INET, SOCK_DGRAM, 17);
    if (sockfd < 0) {
        perror("Error en socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en bind");
        exit(1);
    }
    char buffer[100];

    while (1) {
        // Guardo la IP del que manda para tener a modo de ejemplo, aunque al no mandar ni siquiera un ACK no es necesario.
        bzero(buffer, sizeof(buffer));
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        ssize_t recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (recv_len < 0) {
            perror("Error en recvfrom");
            continue;
        }

        // Crear un mensaje para la cola
        MensajeCola msg;
        msg.mtype = 1; // Tipo de mensaje para productores
        strncpy(msg.texto, buffer, sizeof(msg.texto) - 1);
        msg.texto[sizeof(msg.texto) - 1] = '\0'; // Asegurar terminación nula

        // Enviar el mensaje a la cola
        if (msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
            perror("Error en msgsnd");
            continue;
        }

        printf("Mensaje recibido de productor: %s\n", msg.texto);
    }
}

void thread_handle_consumers()
{
    // Inicio un socket UDP para escuchar a los consumidores
    int sockfd = socket(AF_INET, SOCK_DGRAM, 17);
    if (sockfd < 0) {
        perror("Error en socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8081);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en bind");
        exit(1);
    }

    char buffer[100];

    while (1) {
        // Guardo la IP del que manda para devolver el mensaje que va a consumir
        bzero(buffer, sizeof(buffer));
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        ssize_t recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (recv_len < 0) {
            perror("Error en recvfrom");
            continue;
        }

        // Leo un mensaje para la cola
        MensajeCola msg;
        if (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 0, 0) == -1) {
            perror("Error en msgrcv");
            continue;
        }

        // Envio el mensaje al consumidor
        ssize_t sent_len = sendto(sockfd, msg.texto, strlen(msg.texto), 0, (struct sockaddr *)&client_addr, addr_len);
        if (sent_len < 0) {
            perror("Error en sendto");
            continue;
        }
        
        printf("Mensaje enviado a consumidor: %s\n", msg.texto);
    }
}

int main() {

    // registramos el manejador de señales para Ctrl+C
    signal(SIGINT, handle_sigint);

    printf("Iniciando el administrador de la cola...\n");
    
    // Crea la cola de mensajes que se va a compartir entre los hilos    
    inicializar_cola();

    // Creo los hilos para manejar productores y consumidores
    pthread_t thread_producers, thread_consumers;
    pthread_create(&thread_producers, NULL, (void *)thread_handle_producers, NULL);
    pthread_create(&thread_consumers, NULL, (void *)thread_handle_consumers, NULL);

    // Espero a que los hilos terminen (en este caso, nunca lo harán)
    pthread_join(thread_producers, NULL);
    pthread_join(thread_consumers, NULL);

    return 0;
}
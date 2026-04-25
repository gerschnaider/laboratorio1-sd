#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


int main() {
    // Creo un socket UDP para enviar mensajes al queue_admin
    int sockfd = socket(AF_INET, SOCK_DGRAM, 17);
    if (sockfd < 0) {
        perror("Error en socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);
    
    char buffer[100];
    while (1) {
        // 1. Limpiamos el buffer llenándolo de ceros
        memset(buffer, 0, sizeof(buffer));

        // 2. Copiamos el texto adentro del buffer de forma segura
        strncpy(buffer, "Produciendo algo.", sizeof(buffer) - 1);

        ssize_t sent_len = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent_len < 0) {
            perror("Error en sendto");
            continue;
        }
        printf("Mensaje enviado al queue_admin: %s\n", buffer);
        sleep(2); // Simula el tiempo que tarda en producir algo
    }
}
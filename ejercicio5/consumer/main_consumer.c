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
    server_addr.sin_port = htons(8081);
    
    char buffer[100];
    while (1) {
        memset(buffer, 0, sizeof(buffer));

        //Envio un byte solo en 0 para indicar que quiero consumir algo, el mensaje real lo va a enviar el queue_admin
        ssize_t sent_len = sendto(sockfd, buffer, 1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent_len < 0) {
            perror("Error en sendto");
            continue;
        }
        printf("Mensaje enviado al queue_admin para consumir algo.\n");

        // Espero la respuesta del queue_admin con el mensaje a consumir
        struct sockaddr_in from_addr;
        socklen_t from_len = sizeof(from_addr);
        ssize_t recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&from_addr, &from_len);
        if (recv_len < 0) {
            perror("Error en recvfrom");
            continue;
        }

        printf("Mensaje recibido para consumir: %s\n", buffer);

        sleep(2); // Simula el tiempo que tarda en consumir algo
    }
}

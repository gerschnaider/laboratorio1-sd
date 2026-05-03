#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>

static int64_t timespec_to_ns(struct timespec *ts) {
    return (int64_t) ts->tv_sec * 1000000000LL + ts->tv_nsec;
}

int main()
{
    // Inicio un socket UDP para escuchar aviso del master
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error en socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5102);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en bind");
        exit(1);
    }

    char buffer[1024];
    int64_t current_time_ns;


    while (1) {
        memset(buffer, 0, sizeof(buffer));
        struct sockaddr_in master_addr;
        socklen_t addr_len = sizeof(master_addr);
        ssize_t recv_len = recvfrom(sockfd, &buffer, sizeof(buffer), 0, (struct sockaddr *)&master_addr, &addr_len);
        if (recv_len < 0) {
            perror("Error en recvfrom");
            exit(1);
        }

        printf("Mensaje señal recibido del master");

        // Obtiene el tiempo actual
        struct timespec current_time;
        if (clock_gettime(CLOCK_REALTIME, &current_time) < 0) {
            perror("Error en clock_gettime");
            exit(1);
        }
        current_time_ns = timespec_to_ns(&current_time);
        int64_t network_current_time_ns = htobe64(current_time_ns); // Convertir a orden de red

        // Envia el tiempo al master
        ssize_t sent_len = sendto(sockfd, &network_current_time_ns, sizeof(network_current_time_ns), 0, (struct sockaddr *)&master_addr, addr_len);
        if (sent_len < 0) {
            perror("Error en sendto");
            exit(1);    
        }

        printf("Tiempo enviado al master: %ld ns\n", current_time_ns);

    }


}
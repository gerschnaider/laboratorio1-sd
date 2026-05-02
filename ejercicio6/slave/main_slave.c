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

    int64_t net_buffer, current_time_ns;


    while (1) {
        struct sockaddr_in master_addr;
        socklen_t addr_len = sizeof(master_addr);
        ssize_t recv_len = recvfrom(sockfd, &net_buffer, sizeof(net_buffer), 0, (struct sockaddr *)&master_addr, &addr_len);
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

        int64_t time_diff_ns = current_time_ns - net_buffer;


        // Envia la diferencia de tiempo al master
        ssize_t sent_len = sendto(sockfd, &time_diff_ns, sizeof(time_diff_ns), 0, (struct sockaddr *)&master_addr, addr_len);
        if (sent_len < 0) {
            perror("Error en sendto");
            exit(1);    
        }

        printf("Diferencia de tiempo enviada al master: %ld segundos y %ld nanosegundos\n", time_diff_ns / 1000000000, time_diff_ns % 1000000000);

    }


}
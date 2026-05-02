#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 5101
struct timespec net_buffer;
struct timespec roud_trip_time;

int64_t static timespec_to_ns(struct timespec *ts) {
    return (int64_t)ts->tv_sec * 1000000000LL + ts->tv_nsec;
}

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    int64_t current_time_ns, recieved_diff_tiem_ns;
    socklen_t len;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Fallo en la creación del socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // 2. Configurar la dirección del servidor
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY; // Escuchar en cualquier interfaz
    servaddr.sin_port = htons(PORT); // Puerto convertido a orden de red

    // 3. Vincular el socket al puerto (Bind)
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Fallo en el bind");
        exit(EXIT_FAILURE);
    }

    printf("Servidor UDP en el puerto %d...\n", PORT);


    len = sizeof(cliaddr);

    struct timespec current_time;
    clock_gettime(CLOCK_REALTIME, &current_time);
    current_time_ns = timespec_to_ns(&current_time);

    cliaddr.sin_family = AF_INET; // IPv4
    cliaddr.sin_port = htons(PORT); // Puerto convertido a orden de red
    if(argc < 2) {
        perror("Faltan argumentos: se requiere la dirección IP del cliente");
        exit(EXIT_FAILURE);
    }

    if (inet_pton(AF_INET, argv[1], &cliaddr.sin_addr) <= 0) {
        perror("Dirección IP inválida o no soportada");
        exit(EXIT_FAILURE);
    }

   
    if(sendto(sockfd, &current_time_ns, sizeof(int64_t), 0, (const struct sockaddr *)&cliaddr, len) < 0)
    {
        perror("Fallo en el sendto");
        exit(EXIT_FAILURE);
    }
    
    printf("Tiempo local enviado enviada. \n ");


    if(recvfrom(sockfd, &recieved_diff_tiem_ns, sizeof(int64_t), 0, (struct sockaddr *)&cliaddr, &len) < 0)
    {
        perror("Fallo en el recvfrom");
        exit(EXIT_FAILURE);
    }

    
    printf("Diferencia de tiempo con servidor de tiempo: %lld.%09lld\n", recieved_diff_tiem_ns / 1000000000LL, recieved_diff_tiem_ns % 1000000000LL);

    return 0;
}
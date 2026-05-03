#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdbool.h>

#define CLIENT_PORT 5101
#define BUFFER_SIZE 1024
#define SERVER_PORT 5102
#define MAX_SLAVES 10



typedef struct {
    struct in_addr ip;      // Guarda la IP en formato binario de red
    int64_t offset_ns;      // La diferencia de tiempo que nos mandó
    bool ocupado;           // Flag para saber si esta ranura está en uso
} SlaveInfo;

// Función para guardar o actualizar el dato en la tabla
void guardar_offset(SlaveInfo tabla[], struct in_addr ip_nueva, int64_t offset_nuevo) {
    int ranura_libre = -1;

    for (int i = 0; i < MAX_SLAVES; i++) {
        if (!tabla[i].ocupado) {
            if (ranura_libre == -1) {
                ranura_libre = i;
            }
        }
    }

    // Si llegamos acá, significa que es un esclavo nuevo y no estaba en la tabla
    if (ranura_libre != -1) {
        tabla[ranura_libre].ip = ip_nueva;
        tabla[ranura_libre].offset_ns = offset_nuevo;
        tabla[ranura_libre].ocupado = true;
    } else {
        printf("Error: La tabla está llena. No caben más esclavos.\n");
    }
}

static int64_t timespec_to_ns(struct timespec *ts) {
    return (int64_t)ts->tv_sec * 1000000000LL + ts->tv_nsec;
}

static struct timespec ns_to_timespec(int64_t ns) {
    struct timespec ts;
    ts.tv_sec = ns / 1000000000LL;
    ts.tv_nsec = ns % 1000000000LL;
    return ts;
}


int main(int argc, char *argv[]) {
    SlaveInfo tabla_esclavos[MAX_SLAVES];
    memset(tabla_esclavos, 0, sizeof(tabla_esclavos));
    int64_t net_buffer;

    
    int sockfd;
    struct sockaddr_in clientaddr, serveraddr;
    int64_t current_time_ns;
    int64_t recieved_diff_tiem_ns[MAX_SLAVES];
    socklen_t len;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Fallo en la creación del socket");
        exit(EXIT_FAILURE);
    }

    if(argc < 2) {
        perror("Faltan argumentos: se requiere la dirección IP del cliente");
        exit(EXIT_FAILURE);
    }

    memset(&clientaddr, 0, sizeof(clientaddr));
    memset(&serveraddr, 0, sizeof(serveraddr));

    // 2. Configurar la dirección del servidor
    clientaddr.sin_family = AF_INET; // IPv4
    clientaddr.sin_addr.s_addr = INADDR_ANY; //interfaz
    clientaddr.sin_port = htons(CLIENT_PORT); // Puerto convertido a orden de red

    // 3. Vincular el socket al puerto (Bind)
    if (bind(sockfd, (const struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0) {
        perror("Fallo en el bind");
        exit(EXIT_FAILURE);
    }

    printf("Servidor en el puerto %d...\n", CLIENT_PORT);
    
    struct timespec start_time, end_time;

    
    
    // Configurar la dirección del servidor al que se enviarán los mensajes (los slaves)
    len = sizeof(serveraddr);
    serveraddr.sin_family = AF_INET; // IPv4
    serveraddr.sin_port = htons(5102); // Puerto convertido a orden de red
    for (int i = 0; i < argc - 1; i++) { // Para cada dirección IP de servidor proporcionada en los argumentos
        
        // Pide la hora al servidor
        if (inet_pton(AF_INET, argv[i + 1], &serveraddr.sin_addr) <= 0) {
            perror("Dirección IP inválida o no soportada");
            exit(EXIT_FAILURE);
        }
        clock_gettime(CLOCK_REALTIME, &start_time);
        if(sendto(sockfd, "REQUEST_HOUR", 13, 0, (const struct sockaddr *)&serveraddr, len) < 0)
        {
            perror("Fallo en el sendto");
            exit(EXIT_FAILURE);
        }

        // Recibe la hora del servidor y calcula el tiempo de ida y vuelta
        if (recvfrom(sockfd, &net_buffer, sizeof(net_buffer), 0, (struct sockaddr *)&serveraddr, &len) < 0) {
            perror("Fallo en el recvfrom");
            exit(EXIT_FAILURE);
        }
        net_buffer = (int64_t)be64toh(net_buffer);

        if (clock_gettime(CLOCK_REALTIME, &end_time) < 0) {
            perror("Fallo en el clock_gettime");
            exit(EXIT_FAILURE);
        }
        int64_t rtt = timespec_to_ns(&end_time) - timespec_to_ns(&start_time);

        guardar_offset(tabla_esclavos, serveraddr.sin_addr, timespec_to_ns(&start_time) - net_buffer - rtt / 2);
        printf("Diferencia de tiempo calculada para el slave %s: %ld nanosegundos\n", inet_ntoa(serveraddr.sin_addr), timespec_to_ns(&start_time) - net_buffer - rtt / 2);
    }

    return 0;
}
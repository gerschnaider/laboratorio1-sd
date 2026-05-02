#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 5101
#define MAX_SLAVES 10
struct timespec net_buffer;
struct timespec roud_trip_time;

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

int64_t static timespec_to_ns(struct timespec *ts) {
    return (int64_t)ts->tv_sec * 1000000000LL + ts->tv_nsec;
}

int main(int argc, char *argv[]) {
    SlaveInfo tabla_esclavos[MAX_SLAVES];
    memset(tabla_esclavos, 0, sizeof(tabla_esclavos));
    
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    int64_t current_time_ns;
    int64_t recieved_diff_tiem_ns[MAX_SLAVES];
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

    cliaddr.sin_family = AF_INET; // IPv4
    cliaddr.sin_port = htons(5102); // Puerto convertido a orden de red
    if(argc < 2) {
        perror("Faltan argumentos: se requiere la dirección IP del cliente");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < argc - 1; i++) {
        if (inet_pton(AF_INET, argv[i + 1], &cliaddr.sin_addr) <= 0) {
            perror("Dirección IP inválida o no soportada");
            exit(EXIT_FAILURE);
        }
        clock_gettime(CLOCK_REALTIME, &current_time);
        current_time_ns = timespec_to_ns(&current_time);
        if(sendto(sockfd, &current_time_ns, sizeof(int64_t), 0, (const struct sockaddr *)&cliaddr, len) < 0)
        {
            perror("Fallo en el sendto");
            exit(EXIT_FAILURE);
        }
    }
    
    printf("Tiempo local enviado enviada. \n ");


    for (int i = 0; i < argc - 1; i++) {
        if (recvfrom(sockfd, &recieved_diff_time_ns[i], sizeof(int64_t), 0, (struct sockaddr *)&cliaddr, &len) < 0) {
            perror("Fallo en el recvfrom");
            exit(EXIT_FAILURE);
        }

        recieved_diff_time_ns[i] = (int64_t)be64toh(recieved_diff_time_ns[i]));

        printf("Diferencia de tiempo recibida del slave %s: %ld nanosegundos\n", inet_ntoa(cliaddr.sin_addr), recieved_diff_time_ns[i]);

        guardar_offset(tabla_esclavos, cliaddr.sin_addr, recieved_diff_time_ns[i]);
    }

    return 0;
}
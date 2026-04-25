#include <stdio.h>      /* Para printf() y perror() */
#include <stdlib.h>     /* Para exit() */
#include <unistd.h>     /* Para close() */
#include <strings.h>    /* Para bzero() */
#include <sys/types.h>  /* Tipos de datos base del sistema para sockets */
#include <sys/socket.h> /* Para socket(), connect(), recv() */
#include <netinet/in.h> /* Para struct sockaddr_in, htons() */
#include <netdb.h>      /* Para struct hostent y gethostbyname() */
#include <time.h>       /* Para clock_gettime() */
#include <string.h>     /* Para strcpy() */

#define port 14550 /* el puerto donde se conectara */
#define maxdatasize 100

static double timespec_diff_sec(const struct timespec *start, const struct timespec *end) {
	return (double)(end->tv_sec - start->tv_sec) + (double)(end->tv_nsec - start->tv_nsec) / 1e9;
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;
    char buf[maxdatasize]; /* buffer donde se reciben los datos */
    struct hostent *he; /* se utiliza para convertir el nombre del host a su dirección ip */
    struct sockaddr_in their_addr; /* dirección del server donde se conectara */
    if (argc != 2) 
    {
        perror("No address specified. Exiting...\n");
        exit(1);
    }

    if ((he=gethostbyname(argv[1])) == NULL) {
        perror("gethostbyname: cannot resolve address. Exiting...\n");
        exit(1);
    }

    /* creamos el socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket: cannot create socket. Exiting...\n");
        exit(1);
    }

    /* establecemos their_addr con la dirección del server */
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(port);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(their_addr.sin_zero), 8);
    /* intentamos conectarnos con el servidor */
    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect: cannot connect to server. Exiting...\n");
        exit(1);
    }

    // Medimos el tiempo antes de enviar la solicitud al servidor.
    struct timespec t_start, t_ready;
	if (clock_gettime(CLOCK_MONOTONIC, &t_start) == -1) {
		perror("clock_gettime");
		return 1;
	}


    /* Enviamos una solicitud de 4096 bytes al servidor */
    char request[4096];
    strcpy(request, "Envio de solicitud al servidor");
    if (send(sockfd, request, 4096, 0) == -1)
    {
        perror("send: cannot send data to server. Exiting...\n");
        exit(1);
    }


    /* recibimos los datos del servidor */
    if ((numbytes=recv(sockfd, buf, maxdatasize, 0)) == -1)
    {
        perror("recv: cannot receive data from server. Exiting...\n");
        exit(1);
    }

    if (clock_gettime(CLOCK_MONOTONIC, &t_ready) == -1) {
		perror("clock_gettime");
		return 1;
	}

    double elapsed = timespec_diff_sec(&t_start, &t_ready);

    /* visualizamos lo recibido */
    buf[numbytes] = '\0';
    printf("recibido: %s\n",buf);
    printf("Tiempo transcurrido: %.9f segundos\n", elapsed);
    /* devolvemos recursos al sistema */
    close(sockfd);
    return 0;
}
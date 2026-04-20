#include <stdio.h>      /* Para printf() y perror() */
#include <stdlib.h>     /* Para exit() */
#include <unistd.h>     /* Para close() y fork() */
#include <strings.h>    /* Para bzero() */
#include <sys/types.h>  /* Tipos de datos base del sistema para sockets */
#include <sys/socket.h> /* Para socket(), bind(), listen(), accept(), send() */
#include <netinet/in.h> /* Para struct sockaddr_in, INADDR_ANY, htons(), ntohs() */
#include <arpa/inet.h>  /* Para inet_ntoa() */
#include <sys/wait.h>   /* Para waitpid() y la macro WNOHANG */

#define myport 14550 /*nro de puerto donde se conectaran los clientes*/
#define backlog 10 /* tamaño de la cola de conexiones recibidas, si se pasan los rechaza */

int main()
{
    int sockfd; /* el servidor escuchara por sockfd */
    int newfd; /* las transferencias de datos se realizar mediante newfd */
    struct sockaddr_in my_addr; /* contendrá la dir ip y el nro de puerto local */
    struct sockaddr_in their_addr; //contendrá la dir ip y nro de puerto del cliente
    int sin_size; /* contendra el tamaño de la escructura sockaddr_in */

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(myport); /*debe con. a network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* automáticamente usa ip local */
    bzero(&(my_addr.sin_zero), 8); /* rellena con ceros el resto de la estructura *//* asigna un nombre al socket */
    
    if ( bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    } 

    if (listen(sockfd, backlog) == -1) 
    {
        perror("listen");
        exit(1);
    }

    while(1) /* loop principal del servidor que llama a accept() */
    {
        sin_size = sizeof(struct sockaddr_in);
        
        /* El servidor se bloquea aquí esperando la conexión de un cliente */
        if ((newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
        {
            perror("accept");
            continue;
        }

        printf("server: conexion desde: %s desde puerto: %d \n", 
            inet_ntoa(their_addr.sin_addr),  //da un string con la ip del cliente, network to address.
            ntohs(their_addr.sin_port)); //network to host short

        /* Creamos un proceso hijo para atender a este cliente específico */
        if (!fork()) 
        { 
            close(sockfd); /* El hijo no necesita el socket que escucha peticiones nuevas */


            /* Enviamos los datos mediante el nuevo socket conectado */
            if (send(newfd, "hello, world!\n", 14, 0) == -1)
                perror("send");
            
            close(newfd);   /* Cerramos la conexión con el cliente */
            exit(0);    /* El hijo se suicida tras cumplir su tarea */
        }

        close(newfd); /* El padre cierra su copia del socket conectado */
        /* Cosecha de procesos Zombie */
        while(waitpid(-1, NULL, WNOHANG) > 0);
    }
}
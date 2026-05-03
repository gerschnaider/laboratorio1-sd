#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include "message.h"


int main()
{
	int msqid = msgget((key_t)1234, 0666 | IPC_CREAT);
	
	if(msqid == -1)
	{
		printf("No se pudo crear cola de mensajes\n");
		return 1;
	}
	
	//Creo hijos:
	pid_t pid = fork();
	
	if(pid == 0)
	{
		execv("./ejecutable_cliente.out", NULL);
		exit(1);
	}else{
		pid = fork();
		if(pid == 0)
		{
			execv("./ejecutable_servidor.out", NULL);
			exit(1);
		}
	}
	
	wait(NULL);
	wait(NULL);
	
	msgctl(msqid, IPC_RMID, NULL); //Elimina la cola de mensajes al finalizar
	printf("\nFinalizo ejecucion\n");
	return 0;
}

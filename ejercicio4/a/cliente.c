#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "message.h"
#include <time.h>

struct message msg;

void sumOperation()
{
    msg.type = 4; // Indica que es Suma
    arithmeticFunctions arithmeticData = {
        .operands = {5, 10, 0, 0},
        .operandsAmount = 2,
        .result = 0
    };
    msg.data.arithmetic = arithmeticData;
}

void multiplicationOperation()
{
    msg.type = 5; // Indica que es Multiplicación
    arithmeticFunctions arithmeticData = {
        .operands = {5, 10, 0, 0},
        .operandsAmount = 2,
        .result = 0
    };
    msg.data.arithmetic = arithmeticData;
}
void serverName()
{
    msg.type = 1; // Indica que se pide el nombre del servidor
}
void serverDate()
{
    msg.type = 2; // Indica que se pide la fecha del sistema
}
void decimalToBinary()
{
    msg.type = 3; // Indica que se pide la conversión de decimal a binario
    DecimalToBinary conversionData = {
        .numberToConvert = 10,
        .result = 0
    };
    msg.data.decimalToBinary = conversionData;
}

void ShowServerResult()
{
    switch (msg.type) {
        case 6:
            printf("Nombre del servidor: %s\n", msg.data.text);
            break;
        case 7:
            printf("%s\n", msg.data.text);
            break;
        case 8:
            printf("Número convertido a binario: %s\n", msg.data.decimalToBinary.result);
            break;
        case 9:
            printf("Resultado de la suma: %d\n", msg.data.arithmetic.result);
            break;
        case 10:
            printf("Resultado de la multiplicacion: %d\n", msg.data.arithmetic.result);
            break;
        default:
            printf("Tipo de mensaje desconocido\n");
    }
}

static double timespec_diff_sec(const struct timespec *start, const struct timespec *end) {
    return (double)(end->tv_sec - start->tv_sec) * 1e9 +
           (double)(end->tv_nsec - start->tv_nsec);
}


int main() {
    int msqid = msgget((key_t)1234, 0666);
    if (msqid == -1) {
        printf("No se pudo conectar a la cola de mensajes\n");
        exit(1);
    }
    struct timespec t_start, t_ready;
	if (clock_gettime(CLOCK_MONOTONIC, &t_start) == -1) {
		perror("clock_gettime");
		return 1;
	}
    //sumOperation();
    //multiplicationOperation();
    //serverName();
    //serverDate();
    decimalToBinary();

    //Envia mensaje pidiendo servicio a servidor.
    if (msgsnd(msqid, &msg, MESSAGE_LENGHT, 0) == -1) {
        printf("\nNo se pudo mandar mensaje desde A\n");
        printf("%s\n", strerror(errno));
    }

    //Espero a recibir respuesta del servidor
    if (msgrcv(msqid, &msg, MESSAGE_LENGHT, msg.type + 5, 0) == -1) {
        printf("Error al recibir mensaje en Proceso A\n");
        exit(1);
    }

    ShowServerResult();
    if (clock_gettime(CLOCK_MONOTONIC, &t_ready) == -1) {
		perror("clock_gettime");
		return 1;
	}
    double elapsed_time = timespec_diff_sec(&t_start, &t_ready);
    printf("Tiempo transcurrido: %.0f nanosegundos\n", elapsed_time);
    fflush(stdout);

    return 0;
}

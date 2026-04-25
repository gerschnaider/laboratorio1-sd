#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "message.h"

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
            printf("Número convertido a binario: %d\n", msg.data.decimalToBinary.result);
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

int main() {
    int msqid = msgget((key_t)1234, 0666);
    if (msqid == -1) {
        printf("No se pudo conectar a la cola de mensajes\n");
        exit(1);
    }

    sumOperation();
    //multiplicationOperation();
    //serverName();
    //serverDate();
    //decimalToBinary();

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
    fflush(stdout);

    return 0;
}

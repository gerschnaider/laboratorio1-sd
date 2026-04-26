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

void getServerName()
{
    gethostname(msg.data.text, sizeof(msg.data.text));
}

void getSystemDate()
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    snprintf(msg.data.text, sizeof(msg.data.text), 
                 "Fecha actual: %02d/%02d/%d\n", 
                 tm_info->tm_mday, 
                 tm_info->tm_mon + 1, 
                 tm_info->tm_year + 1900);
}

void decimalToBinary()
{
    int num = msg.data.decimalToBinary.numberToConvert;
    char binary[33]; 

    int pos = 0;

    // Recorremos los 31 bits (asumiendo int de 32 bits positivo)
    for (int i = 31; i >= 0; i--) {
        int bit = (num >> i) & 1;
        if (pos > 0 || bit == 1) { // Evita ceros a la izquierda
            binary[pos++] = bit + '0';
        }
    }
    if (pos == 0) binary[pos++] = '0'; // Caso para el número 0
    binary[pos] = '\0';

    strncpy(msg.data.decimalToBinary.result, binary, sizeof(msg.data.decimalToBinary.result));

}

void sum()
{
    msg.data.arithmetic.result = 0;
    for (int i=0; i < msg.data.arithmetic.operandsAmount; i++) {
        printf("Sumando: %d\n", msg.data.arithmetic.operands[i]);
        msg.data.arithmetic.result += msg.data.arithmetic.operands[i];
    }
    printf("Resultado en server: %d\n", msg.data.arithmetic.result);
}

void multiplication()
{
    msg.data.arithmetic.result = 1;
    for (int i=0; i < msg.data.arithmetic.operandsAmount; i++) {
        printf("Multiplicando: %d\n", msg.data.arithmetic.operands[i]);
        msg.data.arithmetic.result *= msg.data.arithmetic.operands[i];
    }
    printf("Resultado en server: %d\n", msg.data.arithmetic.result);
}

int main() {
    int msqid = msgget((key_t)1234, 0666);
    if (msqid == -1) {
        printf("No se pudo conectar a la cola de mensajes\n");
        exit(1);
    }


    if (msgrcv(msqid, &msg, MESSAGE_LENGHT, 0, 0) == -1) {
        printf("Error al recibir mensaje en Proceso B\n");
        exit(1);
    }

    switch (msg.type) {
        case 1:
            getServerName();
            break;
        case 2:
            getSystemDate();
            break;
        case 3:
            // // Conversión de decimal a binario
            decimalToBinary();
            break;
        case 4:
            // Suma
            sum();
            break;
        case 5:
            // // Multiplicación
            multiplication();
            break;
        default:
            printf("Tipo de mensaje desconocido\n");
    }

    msg.type = msg.type + 5; 
    if (msgsnd(msqid, &msg, MESSAGE_LENGHT, 0) == -1) {
        printf("\nNo se pudo mandar mensaje desde B\n");
        printf("%s\n", strerror(errno));
    }
    printf("Mensaje enviado desde servidor\n");

    return 0;
}

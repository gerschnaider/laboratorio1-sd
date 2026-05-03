#include "proto.h"
#include <time.h>
#include <unistd.h>

char **
getservername_1_svc(void *argp, struct svc_req *rqstp)
{
	static char text[100];
	static char * result = text;

	gethostname(text, sizeof(text));

	return &result;
}

char **
getserverdate_1_svc(void *argp, struct svc_req *rqstp)
{
	static char buffer[64]; // Espacio suficiente para cualquier fecha
    static char *result = buffer;

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    // snprintf es seguro y no reserva memoria dinámica
    snprintf(buffer, sizeof(buffer), "%02d/%02d/%d\n", 
             tm_info->tm_mday, 
             tm_info->tm_mon + 1, 
             tm_info->tm_year + 1900);

    return &result;
}

char **
integertobinary_1_svc(int *argp, struct svc_req *rqstp)
{
	static char binary[33]; 
	static char * binaryNumber = binary;
	int num = *argp; 

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
	return &binaryNumber;
}

int *
addition_1_svc(arithmeticParameters *argp, struct svc_req *rqstp)
{
	static int  result;
	result = 0;
    for (int i=0; i < argp -> operandsAmount; i++) {
        result += argp -> operands[i];
    }
	return &result;
}

int *
multitplication_1_svc(arithmeticParameters *argp, struct svc_req *rqstp)
{
	static int  result;
	result = 1;

    for (int i=0; i < argp -> operandsAmount; i++) {
        result *= argp -> operands[i];
    }

	return &result;
}

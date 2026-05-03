#include "proto.h"
#include <time.h>

CLIENT *clnt;

char * getServerName()
{
	char * *result_1;
	char *getservername_1_arg;
	result_1 = getservername_1((void*)&getservername_1_arg, clnt);
	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	return *result_1;


}
char * getServerDate()
{
	char * *result_2;
	char *getserverdate_1_arg;

	result_2 = getserverdate_1((void*)&getserverdate_1_arg, clnt);
	if (result_2 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	printf("%s",*result_2);
	return *result_2;
} 
char * integerToBinary(int integer)
{
	char * *result_3;
	int  integertobinary_1_arg = integer;

	result_3 = integertobinary_1(&integertobinary_1_arg, clnt);
	if (result_3 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
	printf("%s \n", *result_3);
	return *result_3;
}

int addition(int op1, int op2, int op3, int op4, int amountOp)
{
	int  *result_4;
	arithmeticParameters  addition_1_arg = {
        .operands = {op1, op2, op3, op4},
        .operandsAmount = amountOp,
    };

	result_4 = addition_1(&addition_1_arg, clnt);
	if (result_4 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	return *result_4;
}

int multiplication(int op1, int op2, int op3, int op4, int amountOp)
{
	int  *result_5;

	arithmeticParameters  multitplication_1_arg = {
		.operands = {op1, op2, op3, op4},
        .operandsAmount = amountOp,
	};

	result_5 = multitplication_1(&multitplication_1_arg, clnt);
	if (result_5 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	return *result_5;

}

static double timespec_diff_sec(const struct timespec *start, const struct timespec *end) {
    return (double)(end->tv_sec - start->tv_sec) * 1e9 +
           (double)(end->tv_nsec - start->tv_nsec);
}

int
main (int argc, char *argv[])
{

	char *host;
	struct timespec t_start, t_ready;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	//Creo conexion rpc con servidor
	clnt = clnt_create (host, SERVER_FUNCTIONS, FUNCTIONS_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	//getServerName();
	//getServerDate();
	//integerToBinary(10);
	//multiplication();

	if (clock_gettime(CLOCK_MONOTONIC, &t_start) == -1) {
		perror("clock_gettime");
		return 1;
	}
	printf("Resultado de suma: %d\n", addition(10,15,0,0,2));
	if (clock_gettime(CLOCK_MONOTONIC, &t_ready) == -1) {
		perror("clock_gettime");
		return 1;
	}
    double elapsed_time = timespec_diff_sec(&t_start, &t_ready);
    printf("Tiempo transcurrido: %.0f nanosegundos\n", elapsed_time);

	clnt_destroy (clnt);
	return 0;
}

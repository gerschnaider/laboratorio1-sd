#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define READY_FD 3

static int64_t timespec_diff_nanosec(const struct timespec *start, const struct timespec *end) {
	return (int64_t)(end->tv_sec - start->tv_sec) * 1000000000LL + (int64_t)(end->tv_nsec - start->tv_nsec);
}

int main(int argc, char *argv[]) {
	struct timespec t_start, t_ready;
	if (clock_gettime(CLOCK_MONOTONIC, &t_start) == -1) {
		perror("clock_gettime");
		return 1;
	}

	int status = system("./hijo_exec.out");
	if (status == -1) {
		perror("system falló");
		return 1;
	}	

	if (clock_gettime(CLOCK_MONOTONIC, &t_ready) == -1) {
		perror("clock_gettime");
		return 1;
	}

	int64_t elapsed = timespec_diff_nanosec(&t_start, &t_ready);
	printf("Tiempo desde system() hasta que el hijo terminó: %ld microsegundos\n", elapsed / 1000);

	return 0;
}

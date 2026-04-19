
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <string.h>

static double timespec_diff_sec(const struct timespec *start, const struct timespec *end) {
	return (double)(end->tv_sec - start->tv_sec) + (double)(end->tv_nsec - start->tv_nsec) / 1e9;
}

int main(int argc, char *argv[]) {
	struct timespec t_start, t_ready;
	if (clock_gettime(CLOCK_MONOTONIC, &t_start) == -1) {
		perror("clock_gettime");
		return 1;
	}

	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		return 1;
	} else if (pid == 0) {
		// child
		
        char exec_path[] = "./hijo_exec";
		execv(exec_path, NULL);
    
        //falla el execv
        perror("execv falló");
		_exit(127);
	} else {
		// parent
		
		if (wait(NULL) == -1) {
			perror("wait falló");
			return 1;
		}
		
        if (clock_gettime(CLOCK_MONOTONIC, &t_ready) == -1) {
            perror("clock_gettime");
            return 1;
        }

        double elapsed = timespec_diff_sec(&t_start, &t_ready);
        printf("Tiempo desde fork() hasta que el hijo terminó: %.6f segundos\n", elapsed);
	}
	return 0;
}

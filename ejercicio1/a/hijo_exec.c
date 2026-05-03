#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READY_FD 3

int main(int argc, char *argv[]) {


    int a = 2, b = 3;
    printf("Hijo: imagen cargada, ejecutando trabajo...\n");
    for(int i = 0; i < 5; i++) {
        a = a * b;
        b = a + b;
    }

    FILE* fd;
    if ((fd = fopen("./hijo_exec_text.txt", "w")) == NULL) {
        perror("fopen falló");
        return 1;
    }

    char texto[] = "Hijo: trabajo completado, saliendo.\n";

    ssize_t bytes_escritos = fwrite(texto, sizeof(char), strlen(texto), fd);
    if (bytes_escritos == -1) {
        perror("fwrite falló");
        fclose(fd);
        return -1;
    }

    fclose(fd);
    return 0;
}

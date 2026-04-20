struct entrada {
    int arreglo_A[200];
    int arreglo_B[200];
    char padding[2496]; 
};

struct respuesta {
    int resultado[200];
    char padding[1248];
};

program SUMADOR_PROG {
    version SUMADOR_VERS {
        respuesta SUMAR_ARREGLOS(entrada) = 1;
    } = 1;
} = 0x20000001;

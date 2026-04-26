
typedef struct {
    int operands[4];
    int operandsAmount;
    int result;
} arithmeticFunctions;

typedef struct {
    int numberToConvert;
    char result[33];
} DecimalToBinary;



struct message {
    long type;
    union {
        arithmeticFunctions arithmetic;
        DecimalToBinary decimalToBinary;
        char text[512];
    } data;
};

#define MESSAGE_LENGHT sizeof(struct message) - sizeof(long)

/*Tipos:
    1-> nombre de maquina servidro
    2-> fecha del sistema
    3-> Conversion de decimal a binario
    4-> Suma
    5-> Multiplicacion */
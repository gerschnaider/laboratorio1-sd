struct arithmeticParameters {
    int operands[4];
    int operandsAmount;
};

program SERVER_FUNCTIONS {
    version FUNCTIONS_VERS {
        string GETSERVERNAME(void) = 1;
        string GETSERVERDATE(void) = 2;
        string INTEGERTOBINARY(int) = 3;
        int ADDITION(arithmeticParameters) = 4;
        int MULTITPLICATION(arithmeticParameters) = 5;
    } = 1;
} = 0x20000001;
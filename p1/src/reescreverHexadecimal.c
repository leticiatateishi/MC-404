#include "montador.h"
#include "rotulos.h"
#include <stdio.h>
#include <math.h>
#include <string.h>


char* reescreverHexadecimal (char* hexadecimal){

    char hexadecimalReescrito[4];

    if (strlen(hexadecimal) == 5){
        for (int i = 0; i < 3; i++)
            hexadecimalReescrito[i] = hexadecimal[i+2];
    }

    else{
        int i, j = 0;
        while (hexadecimal[i+2] == '0')
            i++;
        while (j  )
    }

}


char* instrucaoHexadecimal (char* instrucao){
    char hexadecimal[3];

    if (!strcmp(instrucao, "ld"))
        strcpy(hexadecimal, "01");

    else if (!strcmp(instrucao, "ldinv"))
        strcpy(hexadecimal, "02");

    else if (!strcmp(instrucao, "ldabs"))
        strcpy(hexadecimal, "03");

    else if (!strcmp(instrucao, "ldmq"))
        strcpy(hexadecimal, "0A");

    else if (!strcmp(instrucao, "ldmqmx"))
        strcpy(hexadecimal, "09");

    else if (!strcmp(instrucao, "store"))
        strcpy(hexadecimal, "21");

    else if (!strcmp(instrucao, "jump"))
        strcpy(hexadecimal, "0D");

    else if (!strcmp(instrucao, "jumpl"))
        strcpy(hexadecimal, "0F");

    else if (!strcmp(instrucao, "jumpr"))
        strcpy(hexadecimal, "10");

    else if (!strcmp(instrucao, "add"))
        strcpy(hexadecimal, "05");

    else if (!strcmp(instrucao, "addabs"))
        strcpy(hexadecimal, "07");

    else if (!strcmp(instrucao, "sub"))
        strcpy(hexadecimal, "06");

    else if (!strcmp(instrucao, "subabs"))
        strcpy(hexadecimal, "08");

    else if (!strcmp(instrucao, "mult"))
        strcpy(hexadecimal, "0B");

    else if (!strcmp(instrucao, "div"))
    strcpy(hexadecimal, "0C");

    else if (!strcmp(instrucao, "rsh"))
        strcpy(hexadecimal, "15");

    else if (!strcmp(instrucao, "lsh"))
        strcpy(hexadecimal, "14");

    else if (!strcmp(instrucao, "storal"))
        strcpy(hexadecimal, "12");

    else if (!strcmp(instrucao, "storar"))
        strcpy(hexadecimal, "13");

    return hexadecimal;
}


char* decimalHexadecimal (char* decimal){
    char numeroHexadecimal[4];
    char numero[64];
    strcpy(numero, decimal);
    removerAspas(numero);
    int valor = atoi(numero);
    int quociente;
    int posicao = 0;

    do{
        quociente = valor/16;
        resto = valor%16;
        valor = quociente;
        numeroHexadecimal[posicao] = hexadecimal(resto);
        posicao ++;
    } while (quociente != 0);

    numeroHexadecimal[posicao] = '\0';
}


char hexadecimal (int numero){
    if (numero == 10)
        return 'A';
    else if (numero == 11)
        return 'B';
    else if (numero == 12)
        return 'C';
    else if (numero == 13)
        return 'D';
    else if (numero == 14)
        return 'E';
    else if (numero == 15)
        return 'F';
    else
        return (numero + '0');
}

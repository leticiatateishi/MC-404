#include "montador.h"
#include "rotulos.h"
#include <stdio.h>
#include <math.h>
#include <string.h>


char* reescreverDiretiva (char* diretiva){

}


char* reescreverHexadecimal (char* hexadecimal){

    char hexadecimalReescrito[4];

    if (strlen(hexadecimal) == 5){
        for (int i = 0; i < 3; i++)
            hexadecimalReescrito[i] = hexadecimal[i+2];
    }

    else{
        int i = 2;
        while (hexadecimal[i] == '0')
            i++;
        int j = i;
        while (j < strlen(hexadecimal))
            hexadecimalReescrito[j-i] = hexadecimal[j];
    }

    return hexadecimalReescrito;

}


char* reescreverInstrucao (char* instrucao){
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


char* reescreverDecimal (char* decimal){
    char numeroHexadecimal[4];
    int valor = atoi(decimal);
    int quociente;
    int posicao = 0;

    do{
        quociente = valor/16;
        resto = valor%16;
        valor = quociente;
        if (resto >= 10)
            numeroHexadecimal[posicao] = resto - 10 + 'A';
        else
            numeroHexadecimal[posicao] = resto + '0';
        posicao ++;
    } while (quociente != 0);

    numeroHexadecimal[posicao] = '\0';
}


char* incrementarHexadecimal (char* hexadecimal){

}

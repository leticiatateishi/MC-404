#include "montador.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Comentario = 0,
    Palavra incorreta = 1
    Instrucao = 1000,
    Diretiva = 1001,
    DefRotulo = 1002,
    Hexadecimal = 1003,
    Decimal = 1004,
    Nome = 1005
*/

int definirTipoToken (char* palavra){

    //  Comentario
    if(palavra[0] == '#')
        return 0;

    //  Instrucao
    if(eInstrucao(palavra))
        return 1000;

    //  Diretiva
    if(eDiretiva(palavra))
        return 1001;

    //  Definicao de rotulo
    if(eDefinicaoRotulo(palavra))
        return 1002;

    //  Hexadecimal
    if (eHexadecimal(palavra))
        return 1003;

    //  Decimal
    if (palavraEDecimal(palavra))
        return 1004;

    //  Nome
    if (eNome(palavra))
        return 1005;

    return 1;
}

//  Retorna 1 se a palavra for um nome ou 0 caso contrário
int eNome (char* palavra){
    if(!(eDecimal(palavra[0])) && eAlfanumerico(palavra) && palavra[strlen(palavra)-1] != ':' && palavra[0] != '.')
        return 1;
    return 0;
}

//  Retorna 1 se a palavra for uma instrução ou 0 caso contrário
int eInstrucao (char *palavra){
    if (strcmp(palavra, "ld") == 0 || strcmp(palavra, "ldinv") == 0 || strcmp(palavra, "ldabs") == 0 || strcmp(palavra, "ldmq") == 0 ||
        strcmp(palavra, "ldmqmx") == 0 || strcmp(palavra, "store") == 0 || strcmp(palavra, "jump") == 0 || strcmp(palavra, "jumpl") == 0 ||
        strcmp(palavra, "jumpr") == 0 || strcmp(palavra, "add") == 0 || strcmp(palavra, "addabs") == 0 || strcmp(palavra, "sub") == 0 ||
        strcmp(palavra, "subabs") == 0 || strcmp(palavra, "mult") == 0 || strcmp(palavra, "div") == 0 || strcmp(palavra, "lsh") == 0 ||
        strcmp(palavra, "rsh") == 0 || strcmp(palavra, "storal") == 0|| strcmp(palavra, "storar") == 0)  return 1;
    return 0;
}

//  Retorna 1 se a palavra é um rótulo ou 0 caso contrário
int eDefinicaoRotulo (char* palavra){
    if(palavra[strlen(palavra)-1] == ':' && (eDecimal(palavra[0]) == 0) && eAlfanumerico(palavra))
        return 1;
    return 0;
}


//  Retorna 1 se a palavra for composta apenas por caracteres alfanuméricos e '_'
int eAlfanumerico (char* palavra){
    for (int i = 0; i < (strlen(palavra)-1); i++){
        if (!eDecimal(palavra[i]) && palavra[i] != '_' && palavra[i] != 'a' && palavra[i] != 'b' && palavra[i] != 'c' && palavra[i] != 'd' &&
              palavra[i] != 'e' && palavra[i] != 'f' && palavra[i] != 'g' && palavra[i] != 'h' && palavra[i] != 'i' && palavra[i] != 'j' &&
              palavra[i] != 'k' && palavra[i] != 'l' && palavra[i] != 'm' && palavra[i] != 'n' && palavra[i] != 'o' && palavra[i] != 'p' &&
              palavra[i] != 'q' && palavra[i] != 'r' && palavra[i] != 's' && palavra[i] != 't' && palavra[i] != 'u' && palavra[i] != 'v' &&
              palavra[i] != 'w' && palavra[i] != 'x' && palavra[i] != 'y' && palavra[i] != 'z') return 0;
    }
    return 1;
}

//  Retorna 1 se a palavra é uma diretiva ou 0 caso contrário
int eDiretiva (char* palavra){
    if (strcmp(palavra, ".set") == 0 || strcmp(palavra, ".org") == 0 || strcmp(palavra, ".align") == 0 || strcmp(palavra, ".wfill") == 0 ||
        strcmp(palavra, ".word") == 0)
        return 1;
    return 0;
}

//Retorna 1 se a palavra é um numero hexa ou 0 caso contrário
int eHexadecimal(char* palavra){
    for (int i = 2; i < strlen(palavra); i++){
        if (!(eDecimal(palavra[i]) || palavra[i] == 'a' || palavra[i] == 'b' || palavra[i] == 'c'|| palavra[i] == 'd' ||
              palavra[i] == 'e' || palavra[i] == 'f'))
            return 0;
    }
    if (palavra[0] == '0' && palavra[1] == 'x' && strlen(palavra)<13 && strlen(palavra) > 2)
        return 1;
    return 0;
}

//  Retorna 1 se a letra é um decimal ou 0 caso contrário
int eDecimal (char letra){
    if (letra == '1' || letra == '2' || letra == '3' || letra == '4' || letra == '5' || letra == '6' || letra == '7' || letra == '8' ||
          letra == '9' || letra == '0')
        return 1;
    return 0;
}

//  Retorna 1 se a palavra é um decimal ou 0 caso contrário
int palavraEDecimal(char* palavra){
    for(int i = 0; i < strlen(palavra); i++){
        if (eDecimal(palavra[i]) == 0)  return 0;
    }
    return 1;
}

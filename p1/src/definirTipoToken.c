#include "montador.h"
#include "token.h"
#include <stdio.h>

/*  Comentario = 0, X
    * Instrucao = 1000,
    * Diretiva = 1001, X
    DefRotulo = 1002, X
    Hexadecimal = 1003, X
    Decimal = 1004, X
    Nome = 1005
*/

int definirTipoToken (char* palavra){

    //  Diretiva
    if(palavra[0] == '.' && eDiretiva(palavra))
        return 1001;

    //  Comentario
    if(palavra[0] == '#')
        return 0;

    //  Definicao de rotulo
    if(palavra[strlen(palavra)-1] == ":" && palavra[0])
        return 1002

    //  Hexadecimal
    if (eHexadecimal)
        return 1003;

    //  Decimal
    if (eDecimal)
        return 1004;

}


int eDiretiva (char* palavra){
    if (!(strcmp()))
}

//Retorna 1 se a palavra é um numero hexa ou 0 caso contrário
int eHexadecimal(char* palavra){
    if (palavra[0] == '0' && (palavra[1] == 'x' || palavra[1] == 'X') && strlen(palavra)<13){
        for (int i = 2; i < strlen(palavra); i++){
            if (!(eDecimal(palavra[i]) || palavra[i] == 'a' || palavra[i] == 'A' || palavra[i] == 'b' || palavra[i] == 'B' ||
                  palavra[i] == 'c' || palavra[i] == 'C' || palavra[i] == 'd' || palavra[i] == 'D' || palavra[i] == 'e' ||
                  palavra[i] == 'E' || palavra[i] == 'f' || palavra[i] == 'F'))
                return 0;
        }
    }
    return 1;
}

int eDecimal (char* palavra){
    for (int i = 0; i < strlen(palavra); i++){
        if (!(palavra[i] == '1' || palavra[i] == '2' || palavra[i] == '3' || palavra[i] == '4' || palavra[i] == '5' ||
              palavra[i] == '6' || palavra[i] == '7' || palavra[i] == '8' || palavra[i] == '9' || palavra[i] == '0'))
            return 0;
    }
    return 1;
}

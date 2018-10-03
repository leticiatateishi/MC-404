#include "montador.h"
#include "rotulos.h"
#include <stdio.h>


/*  O mapa de memória é composto por 13 dígitos: os 3 primeiros indicam o endereço, e os 10 últimos
 *  representam duas instruções do IAS. */

/*
    ---- Voce deve implementar essa função para a Parte 2! ----
    Utilize os tokens da estrutura de tokens para montar seu código!
    Retorna:
        *  1 caso haja erro na montagem; (imprima o emath.crro em stderr)
        *  0 caso não haja erro.
 */
int emitirMapaDeMemoria()
{
    char mapaDeMemoria[4096];
    int posicao = 0;
    Token token = recuperaToken(0);

    if (strcmp(token.palavra, ".org")){
        mapaDeMemoria[0] = mapaDeMemoria[1] = mapaDeMemoria[2] = '0';
        posicao += 3;
    }

    for (unsigned int i = 0; i < getNumberOfTokens(); i++){

        token = recuperaToken(i);

        /*  Se o token for uma instrução. */
        if (token.tipo == 1000){
            char* instrucao = instrucaoHexadecimal(token.palavra)
            escreverMapaDeMemoria(instrucao, mapaDeMemoria, posicao);
            posicao += 2;
        }

        /*  Se o token for uma diretiva. */
        else if (token.tipo == 1001){

        }

        /*  Se o token for uma definição de rótulo. */
        else if (token.tipo == 1002){

        }

        /*  Se o token for um hexadecimal. */
        else if (token.tipo == 1003){

        }

        /*  Se o token for um decimal. */
        else if (token.tipo == 1004){
            char* hexa = decimalHexadecimal(token.palavra);
            escreverMapaDeMemoria(instrucao, mapaDeMemoria, posicao);
            posicao += 3;
        }

        /*  Se o token for um nome. */
        else if (token.tipo == 1005){

        }
    }

    return 0;
}


void escreverMapaDeMemoria (char* substring, char* mapaDeMemoria, int posicao){
    for(int i = 0; i < strlen(substring); i++){
        mapaDeMemoria[posicao+i] = substring[i];

    }
}

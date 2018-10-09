#include "montador.h"
#include "mapaDeMemoria.h"
#include "rotulos.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

    int posicao = 0;
    char enderecoAtual[] = "000";
    /*  Flag que determina se é a primeira ou segunda vez que montamos o mapa de memória.
     *  É igual a 0 se for a primeira vez, ou seja, devemos armazenar todos os rótulos na lista
     *  que armazena rótulos e suas posições. É igual a 1 se for a segunda vez, ou seja, não
     *  precisamos mais armazenar todos os rótulos. */
    int verificarRotulos = 0;

    for (int j = 0; j < 2; j++){

        //  ESCREVER O ENDEREÇO ATUAL SÓ SE TIVER CERTEZA QUE NÃO HÁ UMA DIRETIVA
        for (unsigned int i = 0; i < getNumberOfTokens(); i++){

            Token token = recuperaToken(i);

            /*  Se o token for uma instrução. */
            if (token.tipo == 1000){
                char* instrucao = reescreverHexadecimal(token.palavra);
                escreverMapaDeMemoria(instrucao, mapaDeMemoria, posicao);
                posicao += 2;
            }

            /*  Se o token for uma diretiva. */
            else if (token.tipo == 1001){

            }

            /*  Se o token for uma definição de rótulo. */
            else if (token.tipo == 1002 && (verificarRotulos == 0)){
                adicionarRotulo(criarRotulo(token.palavra, enderecoAtual));
            }

            // /*  Se o token for um hexadecimal. */
            // else if (token.tipo == 1003){
            //     escreverMapaDeMemoria(token.palavra, mapaDeMemoria, posicao);
            //     posicao += 3;
            // }
            //
            // /*  Se o token for um decimal. */
            // else if (token.tipo == 1004){
            //     char* hexa = decimalHexadecimal(token.palavra);
            //     escreverMapaDeMemoria(instrucao, mapaDeMemoria, posicao);
            //     posicao += 3;
            // }

            // /*  Se o token for um nome. */
            // else if (token.tipo == 1005){
            //     if (verificarRotulos == 0)
            //         escreverMapaDeMemoria("000", mapaDeMemoria, posicao);
            //     else
            //         escreverMapaDeMemoria(getEndereco());
            //     posicao += 3;
            // }

            // if (posicaoPalavra(posicao)){
            //     mapaDeMemoria[posicao] = '\n';
            //     incrementarHexadecimal(enderecoAtual);
            //     if (posicao+1 <= getNumberOfTokens() && (recuperaToken(posicao+1)).tipo != 1001){
            //         escreverMapaDeMemoria(enderecoAtual, mapaDeMemoria, posicao);
            //         escreverMapaDeMemoria(" ", mapaDeMemoria, posicao+3);
            //         posicao += 3;
            //     }
            //     posicao++;
            // }
            // else{
            //     mapaDeMemoria[posicao] = ' ';
            // }
            // posicao++;
        }

        verificarRotulos = 1;

    }

    return 0;
}



void escreverMapaDeMemoria (char* substring, char* mapaDeMemoria, int posicao){
    for(int i = 0; i < strlen(substring); i++){
        mapaDeMemoria[posicao+i] = substring[i];
    }
}


/*  Retorna 1 se a posição atual for o final da palavra da direita, ou 0 se a posição atual
 *  for o final de uma palavra da esquerda.
 *  Para verificar se estamos escrevendo a palavra da esquerda ou da direita:
 *  Verificar se a posicao após a escrita do endereço é múltiplo de 13. Se for, estamos na palavra
 *  da direita e devemos pular uma linha. Se não for, estamos na palavra da esquerda e devemos
 *  adicionar um espaço.
 */
//  int posicaoMultiplaDe (int posicao, int multiplo){
//      int i = 0;
//     while (multiplo*i <= posicao){
//         if (multiplo*i == posicao)
//             return 1;
//         i++;
//     }
//     return 1;
// }


 char* removerDoisPontos (char* diretiva){
    char* diretivaReescrita = malloc(strlen(diretiva)*sizeof(char));
    strcpy(diretivaReescrita, diretiva);
    diretivaReescrita[strlen(diretiva)-1] = '\0';
    return diretivaReescrita;
 }

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

int linhasMapa = 0;


int emitirMapaDeMemoria()
{

    /*  Flag que determina se é a primeira ou segunda vez que montamos o mapa de memória.
     *  É igual a 0 se for a primeira vez, ou seja, devemos armazenar todos os rótulos na lista
     *  que armazena rótulos e suas posições. É igual a 1 se for a segunda vez, ou seja, não
     *  precisamos mais armazenar todos os rótulos. */
    int verificarRotulos = 0;
    Token token;

    for (int j = 0; j < 2; j++){
        int posicao = 0;
        char enderecoAtual[] = "000";
        mapaDeMemoria[0] = '\0';


        if (getNumberOfTokens() > 0){
            token = recuperaToken(0);
            if (token.tipo != 1001 || (token.tipo == 1001 && (strcmp(token.palavra, ".org") != 0))){
                strcat(mapaDeMemoria, enderecoAtual);
                posicao += 3;
            }
        }

        //  ESCREVER O ENDEREÇO ATUAL SÓ SE TIVER CERTEZA QUE NÃO HÁ UMA DIRETIVA
        for (int i = 0; i < getNumberOfTokens(); i++){

            token = recuperaToken(i);
            if (posicaoMultiplaDe(posicao, 13)){
                linhasMapa++;
                if (token.tipo != 1001 || (token.tipo == 1001 && (strcmp(token.palavra, ".org") != 0))) {
                    strcat(mapaDeMemoria, "\n");
                    incrementarHexadecimal(enderecoAtual);
                    printf("endereco atual: %s\n",enderecoAtual);
                    strcat(mapaDeMemoria, enderecoAtual);
                    posicao += 3;
                }
            }


            /*  Se o token for uma instrução. */
            if (token.tipo == 1000){
                char instrucao[3];
                strcpy(instrucao, reescreverInstrucao(token.palavra));
                strcat(mapaDeMemoria, instrucao);
                posicao += 2;
            }

            /*  Se o token for uma diretiva. */
            else if (token.tipo == 1001){
                if(reescreverDiretiva(token.palavra, enderecoAtual, &posicao, &i, verificarRotulos) == 0){
                    fprintf(stderr, "ERRO: Usado mas não definido: %s!\n", token.palavra);
                    return 1;
                }
            }

            /*  Se o token for uma definição de rótulo. */
            else if (token.tipo == 1002 && (verificarRotulos == 0)){
                Rotulo novoRotulo;
                removerDoisPontos(token.palavra);
                strcpy(novoRotulo.endereco, enderecoAtual);
                strcpy(novoRotulo.nome, token.palavra);
                printf("Rotulo adicionado: %s - %s\n", novoRotulo.nome, novoRotulo.endereco);
                adicionarRotulo(novoRotulo);
            }

            /*  Se o token for um hexadecimal. */
            else if (token.tipo == 1003){
                strcat(mapaDeMemoria, token.palavra);
                posicao += 3;
            }

            /*  Se o token for um decimal. */
            else if (token.tipo == 1004){
                char hexa[13];
                sprintf(hexa, "%x", atoi(token.palavra));
                strcat(mapaDeMemoria, hexa);
                posicao += 3;
            }

            /*  Se o token for um nome. */
            else if (token.tipo == 1005){
                if (verificarRotulos == 0)
                    strcat(mapaDeMemoria, "000");
                else{
                    char nomeDefinido[64];
                    char rotuloDefinido[64];
                    strcpy(nomeDefinido, getValor(token.palavra));
                    strcpy(rotuloDefinido, getEndereco(token.palavra));
                    if (nomeDefinido == NULL && rotuloDefinido == NULL)
                        return 0;
                    if (nomeDefinido != NULL)
                        strcat(mapaDeMemoria, nomeDefinido);
                    else
                        strcat(mapaDeMemoria, rotuloDefinido);
                }
                posicao += 3;
            }

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

    // reescreverMapa();
    printf("%s\n", mapaDeMemoria);
    for (int k = 0; k < getNumberOfTokens(); k++)
        free((recuperaToken(k)).palavra);
    return 0;
}


void reescreverMapa(){
    char novoMapa[4096];
    int posOriginal = 0;
    int posNovo = 0;
    for (int i = 0; i < linhasMapa; i++){
        for (int k = 0; k < 3; k++)
            novoMapa[posNovo++] = mapaDeMemoria[posOriginal++];
        for (int k = 0; k < 5; k++){
            novoMapa[posNovo++] = ' ';
            for (int j = 0; j < 2; j++)
                novoMapa[posNovo++] = mapaDeMemoria[posOriginal++];
        }
        novoMapa[posNovo++] = mapaDeMemoria[posOriginal++];
    }
    // mapaDeMemoria[0] = '\0';
    strcpy(mapaDeMemoria, novoMapa);
}


 void removerDoisPontos (char* rotulo){
    char rotuloReescrito[64];
    strcpy(rotuloReescrito, rotulo);
    rotuloReescrito[strlen(rotulo)-1] = '\0';
    strcpy(rotulo, rotuloReescrito);
 }

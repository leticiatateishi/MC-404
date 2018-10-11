#include "montador.h"
#include "mapaDeMemoria.h"
#include "rotulos.h"
#include "processamento.h"
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
    // int verificarRotulos = 0;
    Token token;

    for (int verificarRotulos = 0; verificarRotulos < 2; verificarRotulos++){

        int posicao = 0;
        char enderecoAtual[] = "000";
        mapaDeMemoria[0] = '\0';


        if (getNumberOfTokens() > 0){
            token = recuperaToken(0);
            if (escreverEndereco(0)){
                strcat(mapaDeMemoria, enderecoAtual);
                posicao += 3;
            }
        }

        //  ESCREVER O ENDEREÇO ATUAL SÓ SE TIVER CERTEZA QUE NÃO HÁ UMA DIRETIVA
        for (int i = 0; i < getNumberOfTokens(); i++){

            token = recuperaToken(i);
            if (posicaoMultiplaDe(posicao, 14, 13)){
                linhasMapa++;
                strcat(mapaDeMemoria, "\n");
                if (escreverEndereco(i)) {
                    incrementarHexadecimal(enderecoAtual);
                    strcat(mapaDeMemoria, enderecoAtual);
                    posicao += 3;
                }
                posicao++;
            }


            /*  Se o token for uma instrução. */
            if (token.tipo == 1000){
                reescreverInstrucao(token.palavra);
                // strcat(mapaDeMemoria, instrucao);
                posicao += 2;
            }

            /*  Se o token for uma diretiva. */
            else if (token.tipo == 1001){
                if(reescreverDiretiva(token.palavra, enderecoAtual, &posicao, &i, verificarRotulos, &linhasMapa)){
                    // fprintf(stderr, "ERRO: Usado mas não definido: %s!\n", token.palavra);
                    liberarMemoria();
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
                    if (getValor(token.palavra) == NULL && getEndereco(token.palavra) == NULL){
                        fprintf(stderr, "ERRO: Usado mas não definido: %s!\n", token.palavra);
                        liberarMemoria();
                        return 1;
                    }
                    if (getValor(token.palavra) != NULL)
                        strcat(mapaDeMemoria,  getValor(token.palavra));
                    else
                        strcat(mapaDeMemoria, getEndereco(token.palavra));
                }
                posicao += 3;
            }
            mapaDeMemoria[posicao] = '\0';
            // printf("mapa apos ler o token %d:\n%s\n\n", i, mapaDeMemoria);

        }

    }

    reescreverMapa(linhasMapa);
    printf("%s\n", mapaDeMemoria);
    for (int k = 0; k < getNumberOfTokens(); k++)
        free((recuperaToken(k)).palavra);
    return 0;
}


/*  Retorna 1 se não devemos atualizar o endereco ou 0 caso contrário.
 */
int atualizarEndereco (char* endereco, int i){
    while (i < getNumberOfTokens()-1){
        Token proximo = recuperaToken(i+1);
        if (proximo.tipo == 1000)
            return 0;
        i ++;
    }
    return 1;
}


void reescreverMapa(int linhasMapa){
    char novoMapa[4096];
    int posOriginal = 0;
    int posNovo = 0;
    for (int i = 0; i <= linhasMapa; i++){
        for (int k = 0; k < 3; k++)
            novoMapa[posNovo++] = mapaDeMemoria[posOriginal++];
        for (int k = 0; k < 2; k++){
            novoMapa[posNovo++] = ' ';
            for (int j = 0; j < 2; j++)
                novoMapa[posNovo++] = mapaDeMemoria[posOriginal++];
            novoMapa[posNovo++] = ' ';
            for (int j = 0; j < 3; j++)
                novoMapa[posNovo++] = mapaDeMemoria[posOriginal++];
        }
        novoMapa[posNovo++] = mapaDeMemoria[posOriginal++];
    }
    novoMapa[posNovo] = '\0';
    // mapaDeMemoria[0] = '\0';
    strcpy(mapaDeMemoria, novoMapa);
}


 void removerDoisPontos (char* rotulo){
    char rotuloReescrito[64];
    strcpy(rotuloReescrito, rotulo);
    rotuloReescrito[strlen(rotulo)-1] = '\0';
    strcpy(rotulo, rotuloReescrito);
 }

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


/*  Contador de linhas do mapa de memória.
 */
int linhasMapa = 0;

int emitirMapaDeMemoria()
{

    Token token;

    /*  Indica a posição na string do mapa de memória. A primeira linha corresponde às
     *  posicoes 0 a 13, a segunda linha corresponde as posicoes 14 a 27 e etc */
    int posicao = 0;

    /*  Rodamos o código duas vezes. Na primeira vez, armazenamos os rótulos e seus respectivos
     *  enderecos e os nomes e seus respectivos valores. Na segunda vez, montamos o mapa de
     *  memória substituindo os nomes por seus valores/enderecos. */
    for (int verificarRotulos = 0; verificarRotulos < 2; verificarRotulos++){

        posicao = 0;
        /*  String de 3 chars que armazena o endereco atual em hexadecimal. */
        char enderecoAtual[] = "000";
        mapaDeMemoria[0] = '\0';

        /*  Rotina diferencial para o primeiro token, para escrever o primeiro endereco.
         *  Para os tokens diferentes do primeiro, escrevemos o endereco apenas apos
         *  mudarmos de linha. */
        if (getNumberOfTokens() > 0){
            token = recuperaToken(0);
            if (escreverEndereco(0)){
                strcat(mapaDeMemoria, enderecoAtual);
                posicao += 3;
            }
        }

        /*  Analise para cada token do arquivo. */
        for (int i = 0; i < getNumberOfTokens(); i++){

            token = recuperaToken(i);

            /*  Se a posicao atual for a ultima da linha, ou seja, 13 ou 27 ou 41 ou etc,
             *  incrementamos uma linha e escrevemos o novo endereco se possivel. */
            if (posicaoMultiplaDe(posicao, 14, 13)){
                linhasMapa++;
                strcat(mapaDeMemoria, "\n");
                if (escreverEndereco(i)) {
                    incrementarHexadecimal(enderecoAtual);
                    minusculaParaMaiuscular(enderecoAtual);
                    strcat(mapaDeMemoria, enderecoAtual);
                    posicao += 3;
                }
                posicao++;
            }


            /*  Se o token for uma instrução, a escrevemos no mapa e incrementamos a posicao
             *  de dois. */
            if (token.tipo == 1000){
                reescreverInstrucao(token.palavra, enderecoAtual, &posicao, &linhasMapa);
                posicao += 2;
            }

            /*  Se o token for uma diretiva, verificamos se há erros na montagem.
             *  Retornamos 1 e liberamos a memoria se houver erros ou continuamos a montagem
             *  caso contrario. */
            else if (token.tipo == 1001){
                if(reescreverDiretiva(token.palavra, enderecoAtual, &posicao, &i, verificarRotulos,
                &linhasMapa)){
                    liberarMemoria();
                    return 1;
                }
            }

            /*  Se o token for uma definição de rótulo.
             *  Se for a primeira vez que rodamos o loop, adicionamos o rotulo a lista de rotulos.
             *  Se for a segunda vez, pulamos a definicao de rotulo. */
            else if (token.tipo == 1002 && (verificarRotulos == 0)){
                Rotulo novoRotulo;
                removerDoisPontos(token.palavra);
                strcpy(novoRotulo.endereco, enderecoAtual);
                strcpy(novoRotulo.nome, token.palavra);
                adicionarRotulo(novoRotulo);
            }

            /*  Se o token for um hexadecimal, o reescrevemos para retirar o '0x' e completamos
             *  a palavra com zeros caso o hexadecimal tenha tamanho menor que tres. */
            else if (token.tipo == 1003){
                char hexa[13];
                strcpy(hexa, reescreverHexadecimal(token.palavra));
                if(strlen(hexa) < 3){
                    int i = 0;
                    while (i < 3-strlen(hexa)){
                        mapaDeMemoria[posicao++] = '0';
                        i++;
                    }
                    mapaDeMemoria[posicao] = '\0';
                }
                strcat(mapaDeMemoria, hexa);
                posicao += strlen(hexa);
            }

            /*  Se o token for um decimal, o reescrevemos como um numero hexadecimal e completamos
             *  com zeros caso seu tamanho seja menor que tres. */
            else if (token.tipo == 1004){
                char hexa[13];
                sprintf(hexa, "%x", atoi(token.palavra));
                if(strlen(hexa) < 3){
                    int i = 0;
                    while (i < 3-strlen(hexa)){
                        mapaDeMemoria[posicao++] = '0';
                        i++;
                    }
                    mapaDeMemoria[posicao] = '\0';
                }
                strcat(mapaDeMemoria, hexa);
                posicao += strlen(hexa);
            }

            /*  Se o token for um nome.
             *  Se for a primeira iteracao, apenas escrevemos "000" onde deveriamos escrever
             *  o endereco do rotulo. Se for a segunda iteracao, recuperamos o seu valor e
             *  o escrevemos no mapa de memoria*/
            else if (token.tipo == 1005){
                if (verificarRotulos == 0)
                    strcat(mapaDeMemoria, "000");
                else{
                    /*  Se nao encontrarmos um rotulo ou um nome igual ao do token, imprimimos
                     *  uma mensagem de erro, liberamos a memoria e retornamos um. */
                    if (getValor(token.palavra) == NULL && getEndereco(token.palavra) == NULL){
                        fprintf(stderr, "ERRO: Usado mas não definido: %s\n", token.palavra);
                        liberarMemoria();
                        return 1;
                    }
                    char palavra[64];
                    /*  Se encontrarmos um nome igual ao token */
                    if (getValor(token.palavra) != NULL)
                        strcpy(palavra, getValor(token.palavra));
                    /*  Se encontrarmos um endereco igual ao token */
                    else
                        strcpy(palavra, getEndereco(token.palavra));
                    minusculaParaMaiuscular(palavra);
                    strcat(mapaDeMemoria,  palavra);
                }
                posicao += 3;
            }
            mapaDeMemoria[posicao] = '\0';

        }

    }

    /*  Apos terminar de escrever o mapa de memoria, se nao estivermos em uma posicao multipla
     *  de 14 (ou seja, no final de uma linha, no lugar do '\n'), completamos a linha com zeros. */
    if (!posicaoMultiplaDe(posicao, 14, 0)){
        while (!posicaoMultiplaDe(posicao, 14, 13)){
            mapaDeMemoria[posicao] = '0';
            posicao += 1;
        }
        mapaDeMemoria[posicao] = '\n';
        mapaDeMemoria[posicao] = '\0';
        posicao += 1;
    }

    /*  Reescrevemos o mapa e o imprimimos se ele nao for vazio */
    if (mapaDeMemoria[0] != '\0'){
        reescreverMapa(linhasMapa);
        printf("%s\n", mapaDeMemoria);
    }

    /*  Liberamos a memoria alocada e retornamos zero pois nao houve erros de montagem */
    liberarMemoria();
    return 0;
}


/*  Reescreve o mapa de memoria para ficar no modelo DDD AA AAA AA AAA
 */
void reescreverMapa(int linhasMapa){

    char novoMapa[4096];
    int posOriginal = 0;
    int posNovo = 0;

    for (int i = 0; i <= linhasMapa; i++){
        /*  Escrevemos os 3 chars do endereco */
        for (int k = 0; k < 3; k++)
            novoMapa[posNovo++] = mapaDeMemoria[posOriginal++];
        for (int k = 0; k < 2; k++){
            novoMapa[posNovo++] = ' ';
            /*  Escrevemos os dois chars de um operation code */
            for (int j = 0; j < 2; j++)
                novoMapa[posNovo++] = mapaDeMemoria[posOriginal++];
            novoMapa[posNovo++] = ' ';
            /*  Escrevemos os tres chars de um endereco */
            for (int j = 0; j < 3; j++)
                novoMapa[posNovo++] = mapaDeMemoria[posOriginal++];
        }
        novoMapa[posNovo++] = mapaDeMemoria[posOriginal++];
    }
    novoMapa[posNovo] = '\0';
    strcpy(mapaDeMemoria, novoMapa);
}


/*  Remove os dois pontos de um rotulo, para armazena-lo.
 */
 void removerDoisPontos (char* rotulo){
    char rotuloReescrito[64];
    strcpy(rotuloReescrito, rotulo);
    /*  Retiramos o ultimo char, que representa o ':' */
    rotuloReescrito[strlen(rotulo)-1] = '\0';
    strcpy(rotulo, rotuloReescrito);
 }

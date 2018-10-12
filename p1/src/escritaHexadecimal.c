#include "montador.h"
#include "rotulos.h"
#include "mapaDeMemoria.h"
#include "processamento.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int reescreverDiretiva (char* diretiva, char enderecoAtual[4], int* posicao, int *i, int verificarRotulos, int *linhasMapa){

    if (strcmp(diretiva, ".align") == 0) {
        if (!posicaoMultiplaDe(*posicao, 14, 0)){
            // printf("NAO DEVERIA ENTRAR AQUI\n");
            while (!posicaoMultiplaDe(*posicao, 14, 13)){
                mapaDeMemoria[*posicao] = '0';
                *posicao += 1;
            }
            mapaDeMemoria[*posicao] = '\n';
            (*linhasMapa)++;
            (*posicao)++;
        }
        // while (!posicaoMultiplaDe(*posicao, 14, 13)){
        //     mapaDeMemoria[*posicao] = '0';
        //     *posicao += 1;
        // }
        // mapaDeMemoria[*posicao] = '\n';
        // (*linhasMapa)++;
        // (*posicao)++;
        int numero = (int)strtol(enderecoAtual, NULL, 16);
        // printf("endereco atual em int (784): %d e posicao atual (310: %s)\n", numero, enderecoAtual);
        (*i) ++;
        //  ///////////////////////////////////////////////////////////////////// TEM QUE VER ISSO AI
        do{
            numero++;
        }while (numero % atoi((recuperaToken(*i)).palavra) != 0);
        sprintf(enderecoAtual, "%x", numero);
        if (strlen(enderecoAtual)<3){
            int i = 0;
            while (i < 3-strlen(enderecoAtual)){
                mapaDeMemoria[(*posicao)++] = '0';
                i++;
            }
            mapaDeMemoria[(*posicao)] = '\0';
        }
        strcat(mapaDeMemoria, enderecoAtual);
        (*posicao)+=strlen(enderecoAtual);
    }

    else if (strcmp(diretiva, ".org") == 0){
        printf("posicao: %d\n", *posicao);
        // if (!posicaoMultiplaDe(*posicao, 14, 0)){
        //     while (!posicaoMultiplaDe(*posicao, 14, 13)){
        //         mapaDeMemoria[*posicao] = '0';
        //         *posicao += 1;
        //     }
        //     mapaDeMemoria[*posicao] = '\n';
        //     (*linhasMapa)++;
        //     (*posicao)++;
        // }
        (*i)++;
        Token argumento = recuperaToken(*i);
        // char novoEndereco[4];
        // novoEndereco[0] = '\0';
        // printf("argumento do .org: %s\n", argumento.palavra);
        if (argumento.tipo == 1004)
            sprintf(enderecoAtual, "%x", atoi(argumento.palavra));
            // strcpy(enderecoAtual, reescreverDecimal(argumento.palavra));
        else{
            strcpy(enderecoAtual, reescreverHexadecimal(argumento.palavra));
        }
        printf("endereco atual: %s\n", enderecoAtual);
        if (strlen(enderecoAtual) < 3){
            // printf("novo endereco sera %s(%ld)\n", novoEndereco, strlen(novoEndereco));
            int i = 0;
            while (i < 3-strlen(enderecoAtual)){
                mapaDeMemoria[(*posicao)++] = '0';
                i++;
            }
            mapaDeMemoria[(*posicao)] = '\0';
        }

        strcat(mapaDeMemoria, enderecoAtual);
        (*posicao) += strlen(enderecoAtual);
    }

    else if (strcmp(diretiva, ".set") == 0){
        if (verificarRotulos == 0){
            Token segundoArgumento = recuperaToken(*i+2);
            tipoNome novoNome;
            strcpy(novoNome.nome, (recuperaToken(*i+1)).palavra);
            if (segundoArgumento.tipo == 1004)
                sprintf(novoNome.valor, "%x", atoi(segundoArgumento.palavra));
            else
                strcpy(novoNome.valor, reescreverHexadecimal(segundoArgumento.palavra));
            adicionarNome(novoNome);
            printf("Novo nome adicionado: %s - %s\n", novoNome.nome, novoNome.valor);
        }
        *i += 2;
    }

    else if (strcmp(diretiva, ".word") == 0){
        if (posicaoMultiplaDe(*posicao, 14, 8)){
            fprintf(stderr, "IMPOSSIVEL MONTAR CODIGO!\n");
            return 1;
        }
        char palavraHexa[64];
        palavraHexa[0] = '\0';
        (*i) ++;
        Token argumento = recuperaToken(*i);

        // Hexadecimal
        if (argumento.tipo == 1003){
            strcpy(palavraHexa, reescreverHexadecimal(argumento.palavra));
        }

        // Nome  ou rótulo
        else if (argumento.tipo == 1005){
            if (verificarRotulos == 0)
                strcpy(palavraHexa, "000");
            else{
                if (getValor(argumento.palavra) == NULL && getEndereco(argumento.palavra) == NULL){
                    fprintf(stderr, "ERRO: Usado mas não definido: %s!\n", argumento.palavra);
                    return 1;
                }
                if (getValor(argumento.palavra) != NULL){
                    strcpy(palavraHexa,  getValor(argumento.palavra));
                }
                else{
                    strcpy(palavraHexa, getEndereco(argumento.palavra));
                }
            }
        }

        // Decimal
        else{
            sprintf(palavraHexa, "%x", atoi(argumento.palavra));
        }

        int j = 0;
        // printf("tamanho de %s: %ld\n",palavraHexa, strlen(palavraHexa));
        while (j < 10-strlen(palavraHexa)){
            mapaDeMemoria[(*posicao)] = '0';
            // printf("posicao: %d\n", *posicao);
            // strcat(mapaDeMemoria, "0");
            (*posicao)++;
            j++;
        }
        mapaDeMemoria[(*posicao)] = '\0';
        strcat(mapaDeMemoria, palavraHexa);
        (*posicao) += strlen(palavraHexa);
    }


    else if (strcmp(diretiva, ".wfill") == 0){
        char palavraHexa[64];
        Token argumento = recuperaToken(*i+2);

        // Hexadecimal
        if (argumento.tipo == 1003)
            strcpy(palavraHexa, reescreverHexadecimal(argumento.palavra));

        // Nome
        else if (argumento.tipo == 1005){
            if (verificarRotulos == 0)
                strcpy(palavraHexa, "000");
            else {
                if (getValor(argumento.palavra) == NULL && getEndereco(argumento.palavra) == NULL){
                    fprintf(stderr, "ERRO: Usado mas não definido: %s!\n", argumento.palavra);
                    return 1;
                }
                if (getValor(argumento.palavra) != NULL){
                    strcpy(palavraHexa,  getValor(argumento.palavra));
                }
                else{
                    strcpy(palavraHexa, getEndereco(argumento.palavra));
                }
            }
            // printf("palavra a ser escrita: %s\n", palavraHexa);
        }

        // Decimal
        else
            sprintf(palavraHexa, "%x", atoi(argumento.palavra));

        if (!posicaoMultiplaDe(*posicao, 14, 3)){
            printf("nao deveria entrar aqui para dec: %s, posicao: %d\n", argumento.palavra, *posicao);
            while (!posicaoMultiplaDe(*posicao, 14, 13)){
                mapaDeMemoria[*posicao] = '0';
                (*posicao) += 1;
            }
            // mapaDeMemoria[*posicao] = '\0';
            mapaDeMemoria[*posicao] = '\n';
            (*linhasMapa)++;
            // mapaDeMemoria[*posicao] = '\0';
            incrementarHexadecimal(enderecoAtual);
            strcat(mapaDeMemoria, enderecoAtual);
            (*posicao) += 4;
        }
        for (int k = 0; k < atoi((recuperaToken(*i+1)).palavra); k++){
            int j = 0;
            while (j < 10-strlen(palavraHexa)){
                mapaDeMemoria[*posicao] = '0';
                *posicao += 1;
                j++;
            }
            mapaDeMemoria[*posicao] = '\0';
            strcat(mapaDeMemoria, palavraHexa);
            (*posicao) += strlen(palavraHexa);
            if (k < atoi(recuperaToken(*i+1).palavra)-1){
                mapaDeMemoria[*posicao] = '\n';
                (*posicao)++;
                incrementarHexadecimal(enderecoAtual);
                // mapaDeMemoria[*posicao] = '\0';
                strcat(mapaDeMemoria, enderecoAtual);
                (*posicao) += 3;
            }
            // else{
            //     mapaDeMemoria[*posicao] = '\n';
            // }
        }
        *linhasMapa += atoi(recuperaToken(*i+1).palavra);
        (*i) += 2;
    }
    return 0;
}


char* reescreverHexadecimal (char* hexadecimal){

    static char hexadecimalReescrito[64];

    if (strlen(hexadecimal) == 5){
        int i;
        for (i = 0; i < 3; i++)
            hexadecimalReescrito[i] = hexadecimal[i+2];
        hexadecimalReescrito[i] = '\0';
    }

    else{
        long i = 2;
        while (hexadecimal[i] == '0')
            i++;
        long j = i;
        while (j < strlen(hexadecimal)){
            hexadecimalReescrito[j-i] = hexadecimal[j];
            j++;
        }
        hexadecimalReescrito[j-i] = '\0';
    }

    printf("%s reescrito é %s\n", hexadecimal, hexadecimalReescrito);
    return hexadecimalReescrito;

}



void reescreverInstrucao (char* instrucao, char* enderecoAtual, int* posicao, int* linhasMapa){

    if (!posicaoMultiplaDe(*posicao, 14, 3) && !posicaoMultiplaDe(*posicao, 14, 8)){
        printf("instrucao: %s posicao: %d\n", instrucao, *posicao);
        while (!posicaoMultiplaDe(*posicao, 14, 3) && !posicaoMultiplaDe(*posicao, 14, 8)){
            if (posicaoMultiplaDe(*posicao, 14, 13)){
                mapaDeMemoria[(*posicao)++] = '\n';
                (*linhasMapa)++;
                incrementarHexadecimal(enderecoAtual);
                strcat(mapaDeMemoria, enderecoAtual);
                (*posicao) += 3;
                break;
            }
            mapaDeMemoria[(*posicao)++] = '0';
        }
        mapaDeMemoria[*posicao] = '\0';
    }

    if (!strcmp(instrucao, "ld"))
        strcat(mapaDeMemoria, "01");
        // strcpy(hexadecimal, "01");

    else if (!strcmp(instrucao, "ldinv"))
        strcat(mapaDeMemoria, "02");
        // strcpy(hexadecimal, "02");

    else if (!strcmp(instrucao, "ldabs"))
        strcat(mapaDeMemoria, "03");
        // strcpy(hexadecimal, "03");

    else if (!strcmp(instrucao, "ldmq"))
        strcat(mapaDeMemoria, "0A");
        // strcpy(hexadecimal, "0A");

    else if (!strcmp(instrucao, "ldmqmx"))
        strcat(mapaDeMemoria, "09");
        // strcpy(hexadecimal, "09");

    else if (!strcmp(instrucao, "store"))
        strcat(mapaDeMemoria, "21");
        // strcpy(hexadecimal, "21");

    else if (!strcmp(instrucao, "jump"))
        strcat(mapaDeMemoria, "0D");
        // strcpy(hexadecimal, "0D");

    else if (!strcmp(instrucao, "jumpl"))
        strcat(mapaDeMemoria, "0F");
        // strcpy(hexadecimal, "0F");

    else if (!strcmp(instrucao, "jumpr"))
        strcat(mapaDeMemoria, "10");
        // strcpy(hexadecimal, "10");

    else if (!strcmp(instrucao, "add"))
        strcat(mapaDeMemoria, "05");
        // strcpy(hexadecimal, "05");

    else if (!strcmp(instrucao, "addabs"))
        strcat(mapaDeMemoria, "07");
        // strcpy(hexadecimal, "07");

    else if (!strcmp(instrucao, "sub"))
        strcat(mapaDeMemoria, "06");
        // strcpy(hexadecimal, "06");

    else if (!strcmp(instrucao, "subabs"))
        strcat(mapaDeMemoria, "08");
        // strcpy(hexadecimal, "08");

    else if (!strcmp(instrucao, "mult"))
        strcat(mapaDeMemoria, "0B");
        // strcpy(hexadecimal, "0B");

    else if (!strcmp(instrucao, "div"))
        strcat(mapaDeMemoria, "0C");
        // strcpy(hexadecimal, "0C");

    else if (!strcmp(instrucao, "rsh"))
        strcat(mapaDeMemoria, "15");
        // strcpy(hexadecimal, "15");

    else if (!strcmp(instrucao, "lsh"))
        strcat(mapaDeMemoria, "14");
        // strcpy(hexadecimal, "14");

    else if (!strcmp(instrucao, "storal"))
        strcat(mapaDeMemoria, "12");
        // strcpy(hexadecimal, "12");

    else if (!strcmp(instrucao, "storar"))
        strcat(mapaDeMemoria, "13");
        // strcpy(hexadecimal, "13");
}


void incrementarHexadecimal (char* hexadecimal){

    int numero = (int)strtol(hexadecimal, NULL, 16);
    numero++;
    char novoHexa[4];
    sprintf(novoHexa, "%x", numero);
    long tamanho = strlen(novoHexa);
    hexadecimal[0] = '\0';
    if(tamanho < 3){
        int i = 0;
        while(i < 3-tamanho){
            hexadecimal[i] = '0';
            i++;
        }
        hexadecimal[i] = '\0';
        strcat(hexadecimal, novoHexa);
    }
    else{
        strcpy(hexadecimal, novoHexa);
    }
}


/*  Retorna 1 se a posição atual for o final da palavra da direita, ou 0 se a posição atual
 *  for o final de uma palavra da esquerda.
 *  Para verificar se estamos escrevendo a palavra da esquerda ou da direita:
 *  Verificar se a posicao após a escrita do endereço é múltiplo de 13. Se for, estamos na palavra
 *  da direita e devemos pular uma linha. Se não for, estamos na palavra da esquerda e devemos
 *  adicionar um espaço.
 */
 int posicaoMultiplaDe (int posicao, int multiplo, int posicaoBase){
     int i = 0;
     while ((posicaoBase+(multiplo*i)) <= posicao){
         if ((posicaoBase+(multiplo*i)) == posicao)
             return 1;
         i++;
     }
     return 0;
 }



int escreverEndereco (int i){
    Token token;
    for (int j=i; j < getNumberOfTokens(); j++){
        token = recuperaToken(j);
        if (token.tipo == 1000 || !strcmp(token.palavra, ".word") || !strcmp(token.palavra, ".wfill"))
            return 1;
        if(strcmp(token.palavra, ".org") == 0 || !strcmp(token.palavra, ".align"))
            return 0;
    }
    return 0;
}

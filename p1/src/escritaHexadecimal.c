#include "montador.h"
#include "rotulos.h"
#include "mapaDeMemoria.h"
#include "processamento.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int reescreverDiretiva (char* diretiva, char enderecoAtual[4], int* posicao, int *i, int verificarRotulos,
int *linhasMapa){

    /*  A diretiva .align possui apenas um argumento, um inteiro que indica que o proximo endereco
     *  deve ser um multiplo deste inteiro. */
    if (strcmp(diretiva, ".align") == 0) {

        /*  Como ha apenas um argumento, incrementamos i de um. */
        (*i) ++;

        /*  Se a posicao atual nao for multipla de 14 (isto é, nao é a primeira posicao da linha) */
        if (!posicaoMultiplaDe(*posicao, 14, 0)){

            /*  Completamos a linha com zero, se necessario */
            while (!posicaoMultiplaDe(*posicao, 14, 13))
                mapaDeMemoria[(*posicao)++] = '0';

            /*  Incrementamos uma linha mas nao escrevemos o novo endereco. */
            mapaDeMemoria[*posicao] = '\n';
            (*linhasMapa)++;
            (*posicao)++;
        }

        /*  Reescrevemos o endereco em inteiro para facilitar as contas e o incrementamos ate
         *  que ele seja um multiplo da palavra do token seguinte */
        int numero = (int)strtol(enderecoAtual, NULL, 16);
        do{
            numero++;
        }while (numero % atoi((recuperaToken(*i)).palavra) != 0);

        /*  Copiamos o novo endereco para enderecoAtual, completamos com zeros caso necessario
         *  e escrevemos o novo endereco no mapa de memoria. */
        sprintf(enderecoAtual, "%x", numero);
        minusculaParaMaiuscular(enderecoAtual);
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


    /*  A diretiva .org possui apenas um argumento, que indica o endereco para o qual devemos
     *  pular, que pode estar em hexadecimal ou em decimal */
    else if (strcmp(diretiva, ".org") == 0){

        (*i)++;
        Token argumento = recuperaToken(*i);

        /*  Se o argumento for um decimal, o reescrevemos em hexadecimal. */
        if (argumento.tipo == 1004){
            sprintf(enderecoAtual, "%x", atoi(argumento.palavra));
            minusculaParaMaiuscular(enderecoAtual);
        }

        /*  Se o argumento for um hexadecimal, o reescrevemos para retirar o '0x'. */
        else
            strcpy(enderecoAtual, reescreverHexadecimal(argumento.palavra));

        /*  Se o tamanho do endereco atual for menor do que tres, devemos adicionar zeros
         *  a esquerda ate que o tamanho seja tres. */
        if (strlen(enderecoAtual) < 3){
            int i = 0;
            while (i < 3-strlen(enderecoAtual)){
                mapaDeMemoria[(*posicao)++] = '0';
                i++;
            }
            mapaDeMemoria[(*posicao)] = '\0';
        }

        /*  Escrevemos o endereco novo no mapa de memoria. */
        strcat(mapaDeMemoria, enderecoAtual);
        (*posicao) += strlen(enderecoAtual);
    }


    /*  A diretiva .set possui dois argumentos. O primeiro argumento é um nome e o segundo argumento
     *  é um valor, que pode ser um decimal ou um hexadecimal.*/
    else if (strcmp(diretiva, ".set") == 0){

        /*  Se for a primeira iteracao, armazenamos o novo nome. Caso contrario, ignoramos. */
        if (verificarRotulos == 0){
            Token segundoArgumento = recuperaToken(*i+2);

            /*  Criamos um novo nome, sendo o nome o primeiro argumento de .set e o valor o segundo
             *  argumento de .set*/
            tipoNome novoNome;
            strcpy(novoNome.nome, (recuperaToken(*i+1)).palavra);
            /*  Se for um decimal, basta converter para hexadecimal */
            if (segundoArgumento.tipo == 1004){
                sprintf(novoNome.valor, "%x", atoi(segundoArgumento.palavra));
                minusculaParaMaiuscular(novoNome.valor);
            }
            /*  Se for hexadecimal, basta remover o '0x' */
            else
                strcpy(novoNome.valor, reescreverHexadecimal(segundoArgumento.palavra));
            adicionarNome(novoNome);
        }

        /*  Incrementamos i de 2 pois esta diretiva possui dois argumentos que ja foram analisados */
        *i += 2;
    }


    /*  A diretiva .word possui apenas um argumento, que indica a palavra que sera escrita, podendo
     *  ser um decimal, um hexadecimal ou um nome */
    else if (strcmp(diretiva, ".word") == 0){

        /*  Caso a palavra esteja desalinhada, ou seja, a posicao atual esteja no meio da linha */
        if (posicaoMultiplaDe(*posicao, 14, 8)){
            fprintf(stderr, "IMPOSSIVEL MONTAR CODIGO!\n");
            return 1;
        }

        char palavraHexa[64];
        palavraHexa[0] = '\0';
        (*i) ++;
        Token argumento = recuperaToken(*i);

        /*  Se for um hexadecimal, removemos o '0x' */
        if (argumento.tipo == 1003){
            strcpy(palavraHexa, reescreverHexadecimal(argumento.palavra));
        }

        /*  Se for um nome */
        else if (argumento.tipo == 1005){

            /*  Ignoramos se for a primeira iteracao */
            if (verificarRotulos == 0)
                strcpy(palavraHexa, "000");

            /*  Se for a segunda iteracao, tentamos recuperar o endereco do rotulo ou o valor do nome */
            else{

                /*  Se nao encontrarmos um nome ou um rotulo compativel */
                if (getValor(argumento.palavra) == NULL && getEndereco(argumento.palavra) == NULL){
                    fprintf(stderr, "ERRO: Usado mas não definido: %s!\n", argumento.palavra);
                    return 1;
                }
                /*  Se encontrarmos um nome compativel, armazenamos seu valor */
                if (getValor(argumento.palavra) != NULL)
                    strcpy(palavraHexa,  getValor(argumento.palavra));
                /*  Se encontrarmos um rotulo compativel, pegamos o seu endereco*/
                else
                    strcpy(palavraHexa, getEndereco(argumento.palavra));
            }
        }

        /*  Se for um decimal, apenas o reescrevemos como hexadecimal */
        else{
            sprintf(palavraHexa, "%x", atoi(argumento.palavra));
            minusculaParaMaiuscular(palavraHexa);
        }

        /*  Completamos a palavra com zeros caso necessario */
        int j = 0;
        while (j < 10-strlen(palavraHexa)){
            mapaDeMemoria[(*posicao)] = '0';
            (*posicao)++;
            j++;
        }

        /*  Escrevemos a palavra no mapa de memoria. */
        mapaDeMemoria[(*posicao)] = '\0';
        strcat(mapaDeMemoria, palavraHexa);
        (*posicao) += strlen(palavraHexa);
    }


    /*  A diretiva .wfill recebe dois argumentos. O primeiros argumento é um inteiro que indica a quantidade
     *  de vezes que devemos escrever determinado valor no mapa de memoria. O segundo argumento indica a
     *  palavra que sera escrita, que pode ser um hexadecimal, um decimal ou um nome. */
    else if (strcmp(diretiva, ".wfill") == 0){
        char palavraHexa[64];
        Token argumento = recuperaToken(*i+2);

        /*  Se for um */
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
                    strcpy(palavraHexa, getValor(argumento.palavra));
                }
                else{
                    strcpy(palavraHexa, getEndereco(argumento.palavra));
                }
                minusculaParaMaiuscular(palavraHexa);
            }
        }

        // Decimal
        else{
            sprintf(palavraHexa, "%x", atoi(argumento.palavra));
            minusculaParaMaiuscular(palavraHexa);
        }

        if (!posicaoMultiplaDe(*posicao, 14, 3)){
            while (!posicaoMultiplaDe(*posicao, 14, 13)){
                mapaDeMemoria[*posicao] = '0';
                (*posicao) += 1;
            }
            // mapaDeMemoria[*posicao] = '\0';
            mapaDeMemoria[*posicao] = '\n';
            (*linhasMapa)++;
            incrementarHexadecimal(enderecoAtual);
            minusculaParaMaiuscular(enderecoAtual);
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
                minusculaParaMaiuscular(enderecoAtual);
                strcat(mapaDeMemoria, enderecoAtual);
                (*posicao) += 3;
            }
        }
        *linhasMapa += atoi(recuperaToken(*i+1).palavra);
        (*i) += 2;
    }
    return 0;
}


/*  Reescreve um hexadecimal para remover o '0x'.
 */
char* reescreverHexadecimal (char* hexadecimal){

    static char hexadecimalReescrito[64];

    // if (strlen(hexadecimal) == 5){
    //     int i;
    //     for (i = 0; i < 3; i++)
    //         hexadecimalReescrito[i] = hexadecimal[i+2];
    //     hexadecimalReescrito[i] = '\0';
    // }

    // else{
        long i = 2;
        while (hexadecimal[i] == '0')
            i++;
        long j = i;
        while (j < strlen(hexadecimal)){
            hexadecimalReescrito[j-i] = hexadecimal[j];
            j++;
        }
        hexadecimalReescrito[j-i] = '\0';
    // }

    return hexadecimalReescrito;

}



void reescreverInstrucao (char* instrucao, char* enderecoAtual, int* posicao, int* linhasMapa){

    if (!posicaoMultiplaDe(*posicao, 14, 3) && !posicaoMultiplaDe(*posicao, 14, 8)){
        // printf("instrucao: %s posicao: %d\n", instrucao, *posicao);
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


void minusculaParaMaiuscular (char* palavra){
    int i = 0;

    while (i < strlen(palavra)) {
       if (palavra[i] >= 'a' && palavra[i] <= 'z') {
          palavra[i] -= 32;
       }
       i++;
    }
}

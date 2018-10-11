#include "montador.h"
#include "rotulos.h"
#include "mapaDeMemoria.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int reescreverDiretiva (char* diretiva, char enderecoAtual[4], int* posicao, int *i, int verificarRotulos){

    if (strcmp(diretiva, ".align") == 0) {
        while (!posicaoMultiplaDe(*posicao, 13)){
            mapaDeMemoria[*posicao] = '0';
            *posicao += 1;
        }
        mapaDeMemoria[*posicao] = '\n';
        (*posicao)++;
        int numero = (int)strtol(enderecoAtual, NULL, 16);
        while (numero % atoi((recuperaToken(*i+1)).palavra) != 0)
            numero++;
        sprintf(enderecoAtual, "%x", numero);
    }

    else if (strcmp(diretiva, ".org") == 0){
        if (!posicaoMultiplaDe(*posicao, 14)){
            while (!pularLinha(*posicao)){
                mapaDeMemoria[*posicao] = '0';
                *posicao += 1;
            }
            mapaDeMemoria[*posicao] = '\n';
            (*posicao)++;
        }
        (*i)++;
        Token argumento = recuperaToken(*i);
        if (argumento.tipo == 1004)
            sprintf(enderecoAtual, "%x", atoi(argumento.palavra));
            // strcpy(enderecoAtual, reescreverDecimal(argumento.palavra));
        else{
            strcpy(enderecoAtual, reescreverHexadecimal(argumento.palavra));
            // printf("endereco atual: %s\n", enderecoAtual);
        }
        strcat(mapaDeMemoria, enderecoAtual);
        (*posicao) += 3;
    }

    else if (strcmp(diretiva, ".set") == 0){
        if (verificarRotulos == 0){
            Token segundoArgumento = recuperaToken(*i+2);
            tipoNome novoNome;
            strcpy(novoNome.nome, (recuperaToken(*i+1)).palavra);
            if (segundoArgumento.tipo == 1004)
                sprintf(novoNome.valor, "%x", atoi(segundoArgumento.palavra));
            else
                strcpy(novoNome.valor, segundoArgumento.palavra);
            adicionarNome(novoNome);
            printf("Novo nome adicionado: %s - %s\n", novoNome.nome, novoNome.valor);
        }
        *i += 2;
    }

    else if (strcmp(diretiva, ".word") == 0){
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
                if (getValor(argumento.palavra) == NULL && getEndereco(argumento.palavra) == NULL)
                    return 0;
                if (getValor(argumento.palavra) != NULL){
                    strcpy(palavraHexa,  getValor(argumento.palavra));
                    // printf("nome %s com valor %s encontrado\n", argumento.palavra, palavraHexa);
                }
                else{
                    strcpy(palavraHexa, getEndereco(argumento.palavra));
                }
            }
        }

        // Decimal
        else{
            // printf(".word seguido de decimal\n");
            sprintf(palavraHexa, "%x", atoi(argumento.palavra));

        }
            // strcpy(palavraHexa, reescreverDecimal(argumento.palavra));

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
        // printf("mapa apos escrever uma palavra nova:\n%s\n\n",mapaDeMemoria);
        // printf("palavra a ser escrita: %s\n", palavraHexa);
        (*posicao) += strlen(palavraHexa);
        // printf("mapa apos escrever a palavra %s:\n%s\n\n", palavraHexa, mapaDeMemoria);
    }

    else if (strcmp(diretiva, ".wfill") == 0){
        char palavraHexa[12];
        Token argumento = recuperaToken(*i+2);

        // Hexadecimal
        if (argumento.tipo == 1003)
            strcpy(palavraHexa, argumento.palavra);

        // Nome
        else if (argumento.tipo == 1005){
            if (verificarRotulos == 0)
                strcpy(palavraHexa, "000");
            else if (verificarRotulos == 1){
                char nomeDefinido[64];
                char rotuloDefinido[64];
                strcpy(nomeDefinido, getValor(argumento.palavra));
                strcpy(rotuloDefinido, getEndereco(argumento.palavra));
                if (nomeDefinido == NULL && rotuloDefinido == NULL)
                    return 0;
                if (nomeDefinido != NULL)
                    strcpy(palavraHexa, getValor(argumento.palavra));
                else
                    strcpy(palavraHexa, getEndereco(argumento.palavra));
            }
        }

        // Decimal
        else
            sprintf(palavraHexa, "%x", atoi(argumento.palavra));
            // strcpy(palavraHexa, reescreverDecimal(argumento.palavra));

        while (!posicaoMultiplaDe(*posicao, 13)){
            mapaDeMemoria[*posicao] = '0';
            *posicao += 1;
        }
        int j = 0;

        for (int k = 0; k < atoi(recuperaToken(*i+1).palavra); k++){
            incrementarHexadecimal(enderecoAtual);
            strcat(mapaDeMemoria, enderecoAtual);
            while (j < 10-strlen(palavraHexa)){
                mapaDeMemoria[*posicao] = '0';
                *posicao += 1;
                j++;
            }

            strcat(mapaDeMemoria, palavraHexa);
        }

    }
    return 1;
}


char* reescreverHexadecimal (char* hexadecimal){

    static char hexadecimalReescrito[64];

    if (strlen(hexadecimal) == 5){
        for (int i = 0; i < 3; i++)
            hexadecimalReescrito[i] = hexadecimal[i+2];
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

    // printf("Hexadecimal reescrito (sem o 0x): %s\n", hexadecimalReescrito);
    return hexadecimalReescrito;

}



void reescreverInstrucao (char* instrucao){

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


// char* reescreverDecimal (char* decimal){
//     char hexadecimal[13];
//     sprintf(hexadecimal, "%x", atoi(decimal));
//     char *numeroHexadecimal = malloc(11*sizeof(char));
//     int valor = atoi(decimal);
//     int quociente;
//     int posicao = 0;
//
//     do{
//         quociente = valor/16;
//         int resto = valor%16;
//         valor = quociente;
//         if (resto >= 10)
//             numeroHexadecimal[posicao] = resto - 10 + 'A';
//         else
//             numeroHexadecimal[posicao] = resto + '0';
//         posicao ++;
//     } while (quociente != 0);
//
//     numeroHexadecimal[posicao] = '\0';
//     return numeroHexadecimal;
// }


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
 int posicaoMultiplaDe (int posicao, int multiplo){
     int i = 1;
     while (multiplo*i <= posicao){
         if (multiplo*i == posicao)
             return 1;
         i++;
     }
     return 0;
 }

// }


int pularLinha(int posicao){
    int i = 0;
    while(13+14*i <= posicao){
        if(13+14*i == posicao)
            return 1;
        i++;
    }
    return 0;
}

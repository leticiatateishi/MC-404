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
        int numero = (int)strtol(enderecoAtual, NULL, 16);
        while (numero % atoi((recuperaToken(*i+1)).palavra) != 0)
            numero++;
        sprintf(enderecoAtual, "%x", numero);
        // enderecoAtual = reescreverDecimal(enderecoAtual);
    }

    else if (strcmp(diretiva, ".org") == 0){
        while (!posicaoMultiplaDe(*posicao, 13)){
            mapaDeMemoria[*posicao] = '0';
            *posicao += 1;
        }
        Token argumento = recuperaToken(*i+1);
        if (argumento.tipo == 1004)
            sprintf(enderecoAtual, "%x", atoi(argumento.palavra));
            // strcpy(enderecoAtual, reescreverDecimal(argumento.palavra));
        else
            strcpy(enderecoAtual, argumento.palavra);
    }

    else if (strcmp(diretiva, ".set") == 0){
        if (verificarRotulos == 0){
            // printf("Novo nome adicionado: %s\n", );
            Token segundoArgumento = recuperaToken(*i+2);
            tipoNome novoNome;
            strcpy(novoNome.nome, (recuperaToken(*i+1)).palavra);
            if (segundoArgumento.tipo == 1004)
                sprintf(novoNome.valor, "%x", atoi(segundoArgumento.palavra));
            else
                strcpy(novoNome.valor, segundoArgumento.palavra);
            adicionarNome(novoNome);
        }
        *i += 2;
    }

    else if (strcmp(diretiva, ".word") == 0){
        char palavraHexa[64];
        Token argumento = recuperaToken(*i+1);

        // Hexadecimal
        if (argumento.tipo == 1003){
            strcpy(palavraHexa, reescreverHexadecimal(argumento.palavra));
            // printf(".word seguido de hexadecimal %s de tamanho %ld\n", palavraHexa, strlen(palavraHexa));
        }

        // Nome
        else if (argumento.tipo == 1005){
            if (verificarRotulos == 0)
                strcpy(palavraHexa, "000");
            else{
                if (getValor(argumento.palavra) == NULL)
                    return 0;
                strcpy(palavraHexa, getValor(argumento.palavra));
            }
        }

        // Decimal
        else
            sprintf(palavraHexa, "%x", atoi(argumento.palavra));
            // strcpy(palavraHexa, reescreverDecimal(argumento.palavra));

        int j = 0;
        // printf("tamanho: %ld\n", strlen(palavraHexa));
        while (j < 10-strlen(palavraHexa)){
            strcat(mapaDeMemoria, "0");
            (*posicao)++;
            j++;
        }
        strcat(mapaDeMemoria, palavraHexa);
        // escreverMapaDeMemoria(palavraHexa, *posicao);
        *posicao += strlen(palavraHexa);
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
            else{
                if (getValor(argumento.palavra) == NULL)
                    return 0;
                strcpy(palavraHexa, getValor(argumento.palavra));
            }
        }

        // Decimal
        else
            strcpy(palavraHexa, reescreverDecimal(argumento.palavra));

        while (!posicaoMultiplaDe(*posicao, 13)){
            mapaDeMemoria[*posicao] = '0';
            *posicao += 1;
        }
        int j = 0;

        for (int k = 0; k < atoi(recuperaToken(*i+1).palavra); k++){
            incrementarHexadecimal(enderecoAtual);
            escreverMapaDeMemoria(enderecoAtual, *posicao);
            while (j < 10-strlen(palavraHexa)){
                mapaDeMemoria[*posicao] = '0';
                *posicao += 1;
                j++;
            }
            escreverMapaDeMemoria(palavraHexa, *posicao);
        }

    }
    return 1;
}


void escreverMapaDeMemoria (char* substring, int posicao){
    // for(int i = 0; i < strlen(substring); i++){
    //     mapaDeMemoria[posicao+i] = substring[i];
    // }
    strcat(mapaDeMemoria, substring);
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




char* reescreverInstrucao (char* instrucao){
    char *hexadecimal = malloc(2*sizeof(char));

    if (!strcmp(instrucao, "ld"))
        strcpy(hexadecimal, "01");

    else if (!strcmp(instrucao, "ldinv"))
        strcpy(hexadecimal, "02");

    else if (!strcmp(instrucao, "ldabs"))
        strcpy(hexadecimal, "03");

    else if (!strcmp(instrucao, "ldmq"))
        strcpy(hexadecimal, "0A");

    else if (!strcmp(instrucao, "ldmqmx"))
        strcpy(hexadecimal, "09");

    else if (!strcmp(instrucao, "store"))
        strcpy(hexadecimal, "21");

    else if (!strcmp(instrucao, "jump"))
        strcpy(hexadecimal, "0D");

    else if (!strcmp(instrucao, "jumpl"))
        strcpy(hexadecimal, "0F");

    else if (!strcmp(instrucao, "jumpr"))
        strcpy(hexadecimal, "10");

    else if (!strcmp(instrucao, "add"))
        strcpy(hexadecimal, "05");

    else if (!strcmp(instrucao, "addabs"))
        strcpy(hexadecimal, "07");

    else if (!strcmp(instrucao, "sub"))
        strcpy(hexadecimal, "06");

    else if (!strcmp(instrucao, "subabs"))
        strcpy(hexadecimal, "08");

    else if (!strcmp(instrucao, "mult"))
        strcpy(hexadecimal, "0B");

    else if (!strcmp(instrucao, "div"))
        strcpy(hexadecimal, "0C");

    else if (!strcmp(instrucao, "rsh"))
        strcpy(hexadecimal, "15");

    else if (!strcmp(instrucao, "lsh"))
        strcpy(hexadecimal, "14");

    else if (!strcmp(instrucao, "storal"))
        strcpy(hexadecimal, "12");

    else if (!strcmp(instrucao, "storar"))
        strcpy(hexadecimal, "13");

    return hexadecimal;
}


char* reescreverDecimal (char* decimal){
    char *numeroHexadecimal = malloc(11*sizeof(char));
    int valor = atoi(decimal);
    int quociente;
    int posicao = 0;

    do{
        quociente = valor/16;
        int resto = valor%16;
        valor = quociente;
        if (resto >= 10)
            numeroHexadecimal[posicao] = resto - 10 + 'A';
        else
            numeroHexadecimal[posicao] = resto + '0';
        posicao ++;
    } while (quociente != 0);

    numeroHexadecimal[posicao] = '\0';
    return numeroHexadecimal;
}


void incrementarHexadecimal (char* hexadecimal){

    int numero = (int)strtol(hexadecimal, NULL, 16);
    numero++;
    char novoHexa[4];
    sprintf(novoHexa, "%x", numero);
    long tamanho = strlen(novoHexa);
    // printf("endereco em int: %d e em hexa: %s (%ld)\n", numero, novoHexa, strlen(novoHexa));
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
    // numeroIncrementado = reescreverDecimal(numeroIncrementado);
    // return numeroIncrementado;
}


/*  Retorna 1 se a posição atual for o final da palavra da direita, ou 0 se a posição atual
 *  for o final de uma palavra da esquerda.
 *  Para verificar se estamos escrevendo a palavra da esquerda ou da direita:
 *  Verificar se a posicao após a escrita do endereço é múltiplo de 17. Se for, estamos na palavra
 *  da direita e devemos pular uma linha. Se não for, estamos na palavra da esquerda e devemos
 *  adicionar um espaço.
 */
// int posicaoMultiplaDe (int posicao, int multiplo){
//     int i = 1;
//     while (multiplo*i <= posicao){
//         if (multiplo*i == posicao)
//             return 1;
//         i++;
//     }
//     return 1;
// }

//
// void completarComZero (char* mapaDeMemoria, int posicao, ){
//

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

#include "montador.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Exemplo de erros:
const char* get_error_string (enum errors code) {
    switch (code) {
        case ERR_HEXADECIMAL_NAO_VALIDO:
            return "LEXICO: Numero Hexadecimal Inválido!";
        case ERRO_ROTULO_JA_DEFINIDO:
            return "GRAMATICAL: ROTULO JA FOI DEFINIDO!";
        case ERR_DIRETIVA_NAO_VALIDA:
            return "LEXICO: Diretiva não válida";
*/

/*
    ---- Você Deve implementar esta função para a parte 1.  ----
    Essa função é chamada pela main em main.c
    Entrada da função: arquivo de texto lido e seu tamanho
    Retorna:
        * 1 caso haja erro na montagem; (imprima o erro em stderr)
        * 0 caso não haja erro.         (Caso não haja erro, na parte 1, ao retornar desta função, a lista de Tokens (adicionados utilizando a função adicionarToken()) é impressa)
*/
int processarEntrada(char* entrada, unsigned tamanho)
{
    char palavra[64];
    palavra[0] = '\0';
    int numero_linha = 1;
    unsigned int posicao = 0;

    while(entrada[posicao] != '\0'){
        if (entrada[posicao] == '\n'){
            if (palavra[0] != '\0'){
                int verificacao = verificarPalavra(palavra, numero_linha, entrada, posicao);
                if (verificacao == -1)  return 1;
                else
                    posicao = verificacao;
                palavra[0] = '\0';
            }
            else
                posicao++;
            numero_linha++;
        }
        else if(entrada[posicao] == ' '){
            if(palavra[0] == '\0'){
                posicao++;
                continue;
            }
            int verificacao = verificarPalavra(palavra, numero_linha, entrada, posicao);
            if (verificacao == -1)  return 1;
            else
                posicao = verificacao;
            palavra[0] = '\0';
        }
        else{
            int tamanho_palavra = strlen(palavra);
            palavra[tamanho_palavra] = entrada[posicao];
            palavra[tamanho_palavra+1] = '\0';
            posicao++;
        }
    }
    return 0;
}

Token criarToken(TipoDoToken t, char *p, unsigned l){
    Token novoToken;
    novoToken.tipo = t;
    novoToken.palavra = malloc(strlen(p)+1);
    strcpy(novoToken.palavra, p);
    novoToken.linha = l;
    return novoToken;
}


char* reescreverPalavra (char* palavra){
    for(int i = 0; i < strlen(palavra); i++)
        palavra[i] = maiusculaParaMinuscula(palavra[i]);
    return palavra;
}


int verificarPalavra(char* palavra, int numero_linha, char* entrada, int posicao){
    char palavraReescrita[64];
    strcpy(palavraReescrita, reescreverPalavra(palavra));
    int tipo = definirTipoToken(palavraReescrita);
    if(tipo == 1){
        fprintf(stderr, "ERRO LEXICO: palavra inválida na linha %d!\n", numero_linha);
        return -1;
    }
    else if(tipo == 0){
        while(entrada[posicao] != '\n')
            posicao++;
        return posicao;
    }
    Token novoToken = criarToken(tipo, palavra, numero_linha);
    adicionarToken(novoToken);
    posicao++;
    return posicao;
}

char maiusculaParaMinuscula(char letra){
    switch (letra) {
        case 'A':
            letra = 'a';
            break;
        case 'B':
            letra = 'b';
            break;
        case 'C':
            letra = 'c';
            break;
        case 'D':
            letra = 'd';
            break;
        case 'E':
            letra = 'e';
            break;
        case 'F':
            letra = 'f';
            break;
        case 'G':
            letra = 'g';
            break;
        case 'H':
            letra = 'h';
            break;
        case 'I':
            letra = 'i';
            break;
        case 'J':
            letra = 'j';
            break;
        case 'K':
            letra = 'k';
            break;
        case 'L':
            letra = 'l';
            break;
        case 'M':
            letra = 'm';
            break;
        case 'N':
            letra = 'n';
            break;
        case 'O':
            letra = 'o';
            break;
        case 'P':
            letra = 'p';
            break;
        case 'Q':
            letra = 'q';
            break;
        case 'R':
            letra = 'r';
            break;
        case 'S':
            letra = 's';
            break;
        case 'T':
            letra = 't';
            break;
        case 'U':
            letra = 'u';
            break;
        case 'V':
            letra = 'v';
            break;
        case 'W':
            letra = 'w';
            break;
        case 'X':
            letra = 'x';
            break;
        case 'Y':
            letra = 'y';
            break;
        case 'Z':
            letra = 'z';
            break;
    }
    return letra;
}

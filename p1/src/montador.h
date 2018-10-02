#ifndef MONTADOR_H
#define MONTADOR_H

#include "token.h"

/*
Exemplos de erros e definições...
#define ERROR_OFFSET -1000

enum errors
{
    ERR_HEXADECIMAL_NAO_VALIDO = ERROR_OFFSET,
    ERRO_ROTULO_JA_DEFINIDO,
    ERR_DIRETIVA_NAO_VALIDA,
    ERR_NUMERO_INVALIDO,
    ERR_NOME_INVALIDO,
};
*/

/* Exemplos de funções uteis

    //Retorna 1 se a palavra é um numero hexa ou 0 caso contrário
    int eHexadecimal(char* palavra)

    //Retorna 1 se a palavra é um numero Diretiva ou 0 caso contrário
    int eDiretiva(char* palavra)
*/

int processarEntrada(char*, unsigned);
// Token criarToken(TipoDoToken t, char* p, unsigned l);
// int verificarPalavra(char* palavra, int numero_linha);
// void reescreverPalavra (char* palavra);
// char maiusculaParaMinuscula(char letra);
// void adicionarAspas(char p[64], char novaPalavra[66]);
//
// int definirTipoToken (char* palavra);
// int eNome (char* palavra);
// int eInstrucao (char *palavra);
// int eDefinicaoRotulo (char* palavra);
// int eAlfanumerico (char* palavra);
// int eDiretiva (char* palavra);
// int eHexadecimal(char* palavra);
// int eDecimal(char letra);
// int palavraEDecimal(char* palavra);
// void removerAspas(char* palavra);
//
// int verificarErrosGramaticais();
// int tokenDiretiva (int posicao, Token atual);
// int tokenRotulo (int posicao, Token atual);
// int tokenInstrucao (int posicao, Token atual);

int emitirMapaDeMemoria();
// int definirTipoToken (char* palavra);

#endif

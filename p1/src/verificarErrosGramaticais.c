#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  Retorna 1 se há erro gramatical e 0 caso contrário.
int verificarErrosGramaticais(){
    for(int i = 0; i < getNumberOfTokens(); i++){
        Token atual = recuperaToken(i);
        int argumento;

        //  Se o token for uma diretiva
        if (atual.tipo == 1001){
            argumento = tokenDiretiva(i, atual);
            if (argumento == 0){
                fprintf(stderr, "ERRO GRAMATICAL: palavra na linha %d!\n", atual.linha);
                return 1;
            }
            i += argumento;
        }

        //  Se o token for uma definição de rótulo
        else if (atual.tipo == 1002){
            argumento = tokenRotulo(i, atual);
            if (argumento == 0){
                fprintf(stderr, "ERRO GRAMATICAL: palavra na linha %d!\n", atual.linha);
                return 1;
            }
        }

        else if (atual.tipo == 1000){
            argumento = tokenInstrucao(i, atual);
            if (argumento == 0){
                fprintf(stderr, "ERRO GRAMATICAL: palavra na linha %d!\n", atual.linha);
                return 1;
            }
            else if (argumento == 2)
                i++;
        }

        else{
            fprintf(stderr, "ERRO GRAMATICAL: palavra na linha %d!\n", atual.linha);
            return 1;
        }
    }
    return 0;
}


//  Retorna 0 se houver erro gramatical ou a quantidade de argumentos (1 ou 2) se não houver erro.
int tokenDiretiva (int posicao, Token atual){

    if ((posicao + 1) >= getNumberOfTokens())
        return 0;
    Token argumento1 = recuperaToken(posicao+1);
    if (atual.linha != argumento1.linha)
        return 0;

    if (strcmp (atual.palavra, "\".org\"") == 0){
        if (argumento1.tipo == 1003 || (argumento1.tipo == 1004 && (atoi(argumento1.palavra) <= 1023)))
            return 1;
    }

    else if (strcmp(atual.palavra, "\".word\"") == 0){
        if (argumento1.tipo == 1003 || argumento1.tipo == 1004 || argumento1.tipo == 1005)
            return 1;
    }

    else if (strcmp(atual.palavra, "\".align\"") == 0){
        if (argumento1.tipo == 1004 && (atoi(argumento1.palavra) <= 1023))
            return 1;
    }

    else if (strcmp(atual.palavra, "\".set\"") == 0){
        if ((posicao + 2) >= getNumberOfTokens())
            return 0;
        Token argumento2 = recuperaToken(posicao+2);
        if (argumento1.tipo == 1005 && (argumento2.tipo == 1003 || argumento2.tipo == 1004 ))
            return 2;
    }

    else if (strcmp(atual.palavra, "\".wfill\"") == 0){
        if ((posicao + 2) >= getNumberOfTokens())
            return 0;
        Token argumento2 = recuperaToken(posicao+2);
        char argumento2_palavra[64];
        strcpy(argumento2_palavra, argumento2.palavra);
        removerAspas(argumento2_palavra);
        if (argumento1.tipo == 1004 && ((atoi(argumento2_palavra)) < 1023) && (argumento2.tipo == 1003 || argumento2.tipo == 1004 ||
            argumento2.tipo == 1005))
            return 2;
    }
    return 0;
}


//  Retorna 0 se há erro gramatical e 1 caso contrário.
int tokenRotulo (int posicao, Token atual){

    if (posicao+1 >= getNumberOfTokens())
        return 1;
    Token proximo = recuperaToken(posicao+1);

    if ((atual.linha == proximo.linha) && (proximo.tipo == 1002))
        return 0;

    return 1;
}


void removerAspas(char* palavra){
    for(int i = 0; i < strlen(palavra)-1; i++)
        palavra[i] = palavra[i+1];
    palavra[strlen(palavra)-1] = '\0';
}


//  Retorna 1 se não houver erro gramatical ou 0 caso contrário.
int tokenInstrucao (int posicao, Token atual){
    Token proximo = recuperaToken(posicao+1);
    if (strcmp(atual.palavra, "\"add\"") == 0 || strcmp(atual.palavra, "\"addabs\"") == 0 || strcmp(atual.palavra, "\"sub\"") == 0 ||
    strcmp(atual.palavra, "\"subabs\"") == 0 || strcmp(atual.palavra, "\"mult\"") == 0 || strcmp(atual.palavra, "\"div\"") == 0 ||
    strcmp(atual.palavra, "\"ld\"") == 0 || strcmp(atual.palavra, "\"ldinv\"") == 0 || strcmp(atual.palavra, "\"ldabs\"") == 0 ||
    strcmp(atual.palavra, "\"ldmqmx\"") == 0 || strcmp(atual.palavra, "\"store\"") == 0 || strcmp(atual.palavra, "\"jump\"") == 0 ||
    strcmp(atual.palavra, "\"jumpl\"") == 0 || strcmp(atual.palavra, "\"jumpr\"") == 0 || strcmp(atual.palavra, "\"storal\"") == 0 ||
    strcmp(atual.palavra, "\"storar\"") == 0){
        if(posicao+1 >= getNumberOfTokens())
            return 0;
        Token proximo = recuperaToken(posicao+1);
        if (proximo.tipo == 1003 || proximo.tipo == 1004 || proximo.tipo == 1005)
            return 2;
        return 0;
    }
    if (proximo.linha != atual.linha){
        return 1;
    }
    return 0;

}

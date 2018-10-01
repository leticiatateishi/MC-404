#include "montador.h"
#include "processamento.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Retorna 1 se há erro gramatical e 0 caso contrário.
 *  Verifica o tipo de cada token, quantos argumentos são esperados para esse token e se os argumentos
 *  existentes condizem com os esperados. Se não condizer, há um erro gramatical.
 */
int verificarErrosGramaticais(){

    for(int i = 0; i < getNumberOfTokens(); i++){
        Token atual = recuperaToken(i);
        int argumento;

        /*  Se o token for uma diretiva */
        if (atual.tipo == 1001){
            argumento = tokenDiretiva(i, atual);
            if (argumento == 0){
                fprintf(stderr, "ERRO GRAMATICAL: palavra na linha %d!\n", atual.linha);
                return 1;
            }
            /*  Quando não há erro gramatical, incrementamos i com a quantidade de argumentos que a
             *  diretiva possui. Assim, "pulamos" os argumentos pois estes já foram analisados */
            i += argumento;
        }

        /*  Se o token for uma definição de rótulo */
        else if (atual.tipo == 1002){
            argumento = tokenRotulo(i, atual);
            if (argumento == 0){
                fprintf(stderr, "ERRO GRAMATICAL: palavra na linha %d!\n", atual.linha);
                return 1;
            }
            /*  Como rótulos não possuem argumentos como diretivas, não precisamos incrementar i */
        }

        /*  Se o token for uma instrução */
        else if (atual.tipo == 1000){
            argumento = tokenInstrucao(i, atual);
            if (argumento == 0){
                fprintf(stderr, "ERRO GRAMATICAL: palavra na linha %d!\n", atual.linha);
                return 1;
            }
            /*  Quando não há erro gramatical, temos duas opções: a instrução possui apenas um
             *  argumento ou a instrução possui nenhum argumento. No primeiro caso, a função
             *  tokenInstrucao retorna 2 e devemos ignorar o argumento, pois ele já foi analisado,
             *  portanto, incrementamos i. No segundo caso, a função retorna 1 e não precisamos
             *  saltar nenhum argumento, então apenas prosseguimos */
            else if (argumento == 2)
                i++;
        }

        /*  Se o token for um nome, um decimal ou um hexadecimal */
        else{
            fprintf(stderr, "ERRO GRAMATICAL: palavra na linha %d!\n", atual.linha);
            return 1;
        }
    }
    return 0;
}


/*  Retorna 0 se houver erro gramatical ou a quantidade de argumentos (1 ou 2) se não houver erro.
 */
int tokenDiretiva (int posicao, Token atual){

    /*  Como toda diretiva possui pelo menos um argumento, retornamos 0 se não existir token
     *  após a diretiva */
    if ((posicao + 1) >= getNumberOfTokens())
        return 0;

    Token argumento1 = recuperaToken(posicao+1);

    /*  Retornamos 0 caso o próximo token estiver em linha diferente da atual pois, neste caso,
     *  o próximo token não será argumento da diretiva */
    if (atual.linha != argumento1.linha)
        return 0;

    /*  Retorna 1 se o argumento de .org for um hexadecimal ou um decimal menor que 1023 */
    if (strcmp (atual.palavra, "\".org\"") == 0){
        if (argumento1.tipo == 1003 || (argumento1.tipo == 1004 && !decimalMaiorQue1023(argumento1)))
            return 1;
    }

    /*  Retorna 1 se o argumento de .word for um hexadecimal, um decimal ou um nome */
    else if (strcmp(atual.palavra, "\".word\"") == 0){
        if (argumento1.tipo == 1003 || argumento1.tipo == 1004 || argumento1.tipo == 1005)
            return 1;
    }

    /*  Retorna 1 se o argumento de .align for um decimal menor que 1023*/
    else if (strcmp(atual.palavra, "\".align\"") == 0){
        if (argumento1.tipo == 1004 && !decimalMaiorQue1023(argumento1))
            return 1;
    }

    /*  Retorna 1 se o primeiro argumento de .set for um nome e o segundo argumento for um decimal
     *  ou um hexadecimal */
    else if (strcmp(atual.palavra, "\".set\"") == 0){

        /*  Verifica se existe um segundo argumento */
        if ((posicao + 2) >= getNumberOfTokens())
            return 0;

        Token argumento2 = recuperaToken(posicao+2);

        if (argumento1.tipo == 1005 && (argumento2.tipo == 1003 || argumento2.tipo == 1004 ))
            return 2;
    }

    /*  Retorna 1 se o primeiro argumento de .wfill for um decimal menor do que 1023 e se o segundo
     *  argumento um hexadecimal, um decimal ou um nome */
    else if (strcmp(atual.palavra, "\".wfill\"") == 0){

        /*  Verifica se existe um segundo argumento */
        if ((posicao + 2) >= getNumberOfTokens())
            return 0;

        Token argumento2 = recuperaToken(posicao+2);

        if (argumento1.tipo != 1004)
            return 0;

        else if (decimalMaiorQue1023(argumento1))
            return 0;

        if (argumento2.tipo == 1003 || argumento2.tipo == 1004 || argumento2.tipo == 1005)
            return 2;
    }

    return 0;
}


/*  Retorna 0 se há erro gramatical no rótulo e 1 caso contrário. */
int tokenRotulo (int posicao, Token atual){

    if (posicao+1 >= getNumberOfTokens())
        return 1;
    Token proximo = recuperaToken(posicao+1);

    /*  Um rótulo só pode ser seguido na mesma linha por outro rótulo */
    if ((atual.linha == proximo.linha) && (proximo.tipo == 1002))
        return 0;

    /*  Ignoramos se o próximo token estiver em outra linha */
    return 1;
}


/*  Retorna 1 se a palavra do token for um decimal maior que 1023 ou 0 caso contrário
 */
int decimalMaiorQue1023 (Token token){
    char palavra[64];
    strcpy(palavra, token.palavra);
    removerAspas(palavra);
    if (atoi(palavra) > 1023)
        return 1;
    return 0;
}


/*  Recebe como parâmetro uma palavra com aspas e retorna a mesma palavra sem aspas
 */
void removerAspas(char* palavra){
    for(int i = 0; i < strlen(palavra)-2; i++)
        palavra[i] = palavra[i+1];
    palavra[strlen(palavra)-1] = '\0';
}


/*  Retorna 0 se houver erro gramatical, 1 se a instrução não possuir argumento e 2 se
 *  a instrução possuir apenas um argumento
 */
int tokenInstrucao (int posicao, Token atual){

    Token proximo = recuperaToken(posicao+1);

    /*  Se a instrução for uma das seguintes, ela deve possuir apenas um argumento que represente
     *  M[x], ou seja, este argumento deve ser um decimal, um hexadecimal ou um nome */
    if (strcmp(atual.palavra, "\"add\"") == 0 || strcmp(atual.palavra, "\"addabs\"") == 0 ||
    strcmp(atual.palavra, "\"sub\"") == 0 || strcmp(atual.palavra, "\"subabs\"") == 0 ||
    strcmp(atual.palavra, "\"mult\"") == 0 || strcmp(atual.palavra, "\"div\"") == 0 ||
    strcmp(atual.palavra, "\"ld\"") == 0 || strcmp(atual.palavra, "\"ldinv\"") == 0 ||
    strcmp(atual.palavra, "\"ldabs\"") == 0 || strcmp(atual.palavra, "\"ldmqmx\"") == 0 ||
    strcmp(atual.palavra, "\"store\"") == 0 || strcmp(atual.palavra, "\"jump\"") == 0 ||
    strcmp(atual.palavra, "\"jumpl\"") == 0 || strcmp(atual.palavra, "\"jumpr\"") == 0 ||
    strcmp(atual.palavra, "\"storal\"") == 0 || strcmp(atual.palavra, "\"storar\"") == 0){

        if(posicao+1 >= getNumberOfTokens())
            return 0;
        Token proximo = recuperaToken(posicao+1);

        /*  Retorna 2 se não há erro gramatical e se o token é uma instrução com apenas um
         *  argumento */
        if (proximo.tipo == 1003 || proximo.tipo == 1004 || proximo.tipo == 1005)
            return 2;
        return 0;
    }

    /*  Retorna 1 se o token for uma instrução que não precisa de argumento */
    if (proximo.linha != atual.linha)
        return 1;

    return 0;

}

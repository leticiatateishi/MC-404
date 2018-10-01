#include "montador.h"
#include "processamento.h"
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
        * 0 caso não haja erro.         (Caso não haja erro, na parte 1, ao retornar desta função, a lista de Tokens
        (adicionados utilizando a função adicionarToken()) é impressa)
*/
int processarEntrada(char* entrada, unsigned tamanho){
    char palavra[64] = "\0";
    int numero_linha = 1;
    unsigned int posicao = 0;

    /*  Verificamos todas as letras da entrada, separando-as em palavras. */
    while(posicao < tamanho){

        /*  Ao atingirmos o final de uma linha, devemos finalizar a palavra anterior (se ela existir) e incrementar
         * o contador de linhas. */
        if (entrada[posicao] == '\n'){

            /*  Finaliza a palavra, verifica de qual tipo ela é e cria um novo token*/
            if (palavra[0] != '\0'){
                int verificacao = verificarPalavra(palavra, numero_linha);
                if (verificacao == 1)  return 1;
                else
                    posicao++;
                palavra[0] = '\0';
            }

            /* Se a linha não termina com uma palavra válida, ou seja, termina com um comentário ou um espaço
             * (palavra[0] == '\0' indica que não há palavra armazenada atualmente), prosseguimos */
            else
                posicao++;

            numero_linha++;
        }

        /*  Ao encontrarmos um espaço, devemos finalizar a palavra (se ela existir) ou ignorá-lo, se não houver
         *  palavra atualmente */
        else if(entrada[posicao] == ' '){

            /*  Se não houver palavra atualmente, rosseguimos */
            if(palavra[0] == '\0'){
                posicao++;
                continue;
            }

            /*  Se houver palavra, verificamos de qual tipo ela é, criamos um novo token e a "zeramos" para podermos
             *  armazenar a próxima palavra */
            int verificacao = verificarPalavra(palavra, numero_linha);
            if (verificacao == 1)  return 1;
            else
                posicao ++;
            palavra[0] = '\0';
        }

        /*  Ao encontrarmos um comentário, devemos ignorar todas as letras e palavras após o caracter '#' */
        else if (entrada[posicao] == '#'){
            while (entrada[posicao] != '\n')
                posicao++;
        }

        /*  Se estivermos no meio de uma palavra, devemos transcrever o caracter atual (entrada[posicao])
         *  para a string 'palavra' e incrementar a posicao */
        else{
            int tamanho_palavra = strlen(palavra);
            palavra[tamanho_palavra] = entrada[posicao];
            palavra[tamanho_palavra+1] = '\0';
            posicao++;
        }
    }

    return verificarErrosGramaticais();
}


/*  Recebe como parâmetro os atributos de um novo token a ser criado. Cria o token e adiciona aspas à palavra.
 *  Retorna o token criado.
 *  Essa função só é chamada após a verificação de erros léxicos
 */
Token criarToken(TipoDoToken t, char p[64], unsigned l){
    Token novoToken;
    novoToken.tipo = t;
    novoToken.linha = l;
    novoToken.palavra = malloc(strlen(p)+3);
    char novaPalavra[66] = "\"";
    adicionarAspas(p, novaPalavra);
    strcpy(novoToken.palavra, novaPalavra);
    return novoToken;
}


/*  Recebe a palavra original e uma string com apenas aspas.
 *  Retorna a string reescrita com aspas no começo e no final da palavra.
 */
void adicionarAspas(char p[64], char novaPalavra[66]){
    strcat(novaPalavra, p);
    char aspas[2] = "\"";
    strcat(novaPalavra, aspas);
}


/*  Recebe uma string que pode possuir letras maiúsculas ou minúsculas.
 *  Reescreve a string, convertendo todas as letras maiúsculas em minúsculas
 */
void reescreverPalavra (char *palavra){
    for(int i = 0; i < strlen(palavra); i++)
        palavra[i] = maiusculaParaMinuscula(palavra[i]);
}


/*  Verifica se há erro léxico em cada palavra lida. Se não houver, cria um novo token com essa palavra.
 *  Recebe como parâmetros a palavra e o número da linha (que permitem a verificação de erros léxicos e a
 *  criação de um novo token).
 *  Retorna 1 se houver erro léxico ou 0 se não houver erro léxico.
*/
int verificarPalavra(char* palavra, int numero_linha){

    /*  Reescreve a palavra e determina o seu tipo */
    reescreverPalavra(palavra);
    int tipo = definirTipoToken(palavra);

    /*  Retorna 1 se a palavra não se encaixar nos tipos possíveis */
    if(tipo == 1){
        fprintf(stderr, "ERRO LEXICO: palavra inválida na linha %d!\n", numero_linha);
        return 1;
    }

    /*  Cria um novo token com os atributos recebidos como parâmetros da função e o adiciona à lista
     *  de tokens. Retorna 0 pois não há erro léxico  */
    Token novoToken = criarToken(tipo, palavra, numero_linha);
    adicionarToken(novoToken);
    return 0;
}


/*  Recebe um char (uma letra minúscula ou maiúscula).
 *  Retorna o mesmo char em letra minúscula.
 */
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
        default:
            break;
    }
    return letra;
}

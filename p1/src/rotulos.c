#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"
#include "rotulos.h"

static Rotulo rotulos[4096];
static unsigned tamRotulos = 0;

static tipoNome nomes[4096];
static unsigned tamNomes = 0;


/*  Retorna a quantidade de rotulos ja registrados.
 */
unsigned getNumberOfRotulos() {
  return tamRotulos;
}

/*  Recebe como parametro uma posicao na lista de rotulos e retorna o rotulo que esta naquela posicao.
 */
Rotulo getRotulo(int posicao){
    return rotulos[posicao];
}

/*  Recebe um rotulo novo e o adiciona a lista de rotulos ja existente.
 */
void adicionarRotulo(Rotulo rotulo){
    rotulos[tamRotulos] = rotulo;
    tamRotulos++;
}

/*  Recebe o nome de um rotulo e retorna o seu endereco.
 */
char* getEndereco (char* palavra){
    for (int i = 0; i < getNumberOfRotulos(); i++){
        if (strcmp(palavra, rotulos[i].nome) == 0)
            return rotulos[i].endereco;
    }
    return NULL;
}

/*  Recebe o nome de um rotulo e retorna 1 se esse rotulo estiver numa palavra da esquerda.
 */
int getFlag_esquerda (char* palavra){
    Rotulo rotulo;
    for (int i = 0; i < getNumberOfRotulos(); i++){
        rotulo = recuperaRotulo(i);
        if (strcmp(rotulo.nome, palavra)){
            break;
        }
    }
    return rotulo.flag_esquerda;
}

/*  Retorna o rotulo pertencente a determinada posicao.
 */
Rotulo recuperaRotulo(int posicao){
    return rotulos[posicao];
}

/*  Retorna a quantidade de nomes ja registrados.
 */
unsigned getNumberOfNomes() {
  return tamNomes;
}

/*  Recebe como parametro uma posicao na lista de nomes e retorna o nome que esta naquela posicao.
 */
tipoNome getNome(int posicao){
    return nomes[posicao];
}

/*  Recebe um nome novo e o adiciona a lista de nomes ja existente.
 */
void adicionarNome(tipoNome nome){
    nomes[tamNomes] = nome;
    tamNomes++;
}

/*  Recebe como parametro uma posicao na lista de nomes e retorna o nome que esta naquela posicao.
 */
char* getValor (char* nome){
    for (int i = 0; i < getNumberOfNomes(); i++){
        if (strcmp(nome, nomes[i].nome) == 0)
            return nomes[i].valor;
    }
    return NULL;
}

/*  Retorna o nome pertencente a determinada posicao.
 */
tipoNome recuperaNome(int posicao){
    return nomes[posicao];
}

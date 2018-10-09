#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"
#include "rotulos.h"

static Rotulo rotulos[4096];
static unsigned tamRotulos = 0;

static tipoNome nomes[4096];
static unsigned tamNomes = 0;

unsigned getNumberOfRotulos() {
  return tamRotulos;
}

Rotulo getRotulo(int posicao){
    return rotulos[posicao];
}

void adicionarRotulo(Rotulo* rotulo){
    rotulos[tamRotulos] = *rotulo;
    tamRotulos++;
}

Rotulo* criarRotulo(char* palavra, char* endereco){
    Rotulo *novoRotulo = malloc(sizeof(Rotulo));
    strcpy(novoRotulo->nome, palavra);
    strcpy(novoRotulo->endereco, endereco);
    return novoRotulo;
}

char* getEndereco (char* palavra){
    for (int i = 0; i < getNumberOfRotulos(); i++){
        if (strcmp(palavra, rotulos[i].nome) == 0)
            return rotulos[i].endereco;
    }
    return NULL;
}


unsigned getNumberOfNomes() {
  return tamNomes;
}

tipoNome getNome(int posicao){
    return nomes[posicao];
}

void adicionarNome(tipoNome* nome){
    nomes[tamNomes] = *nome;
    tamNomes++;
}

tipoNome* criarNome(char* palavra, char* valor){
    tipoNome *novoNome = malloc(sizeof(tipoNome));
    strcpy(novoNome->nome, palavra);
    strcpy(novoNome->valor, valor);
    return novoNome;
}

char* getValor (char* nome){
    for (int i = 0; i < getNumberOfNomes(); i++){
        if (strcmp(nome, nomes[i].nome) == 0)
            return nomes[i].valor;
    }
    return NULL;
}

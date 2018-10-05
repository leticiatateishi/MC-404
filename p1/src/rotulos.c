#include <stdio.h>
#include "token.h"
#include "rotulos.h"

static Rotulo rotulos[4096];
static unsigned tamRotulos = 0;

unsigned getNumberOfRotulos() {
  return tamRotulos;
}

Rotulo getRotulo(int posicao){
    if (posicao >= tamRotulos)
        return NULL;
    return rotulos[posicao];
}

void adicionarRotulo(Rotulo rotulo){
    rotulos[tamRotulos] = rotulo;
    tamRotulos++;
}

Rotulo criarRotulo(char* palavra, char* endereco){
    Rotulo novoRotulo;
    strcpy(novoRotulo.nome, palavra);
    strcpy(novoRotulo.endereco, endereco);
    return novoRotulo;
}

char* getEndereco (char* palavra){
    for (int i = 0; i < getNumberOfRotulos(); i++){
        if (strcmp(palavra, rotulos[i].nome) == 0)
            return rotulos[i].endereco;
    }
    return NULL;
}

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
}

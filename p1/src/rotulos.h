#ifndef ROTULOS_H
#define ROTULOS_H

typedef struct Rotulo{
    char* nome;
    char* endereco;
}Rotulo;

unsigned getNumberOfRotulos();

Rotulo getRotulo(int posicao);

void adicionarRotulo(Rotulo rotulo);

Rotulo criarRotulo(char* palavra, char* endereco);

char* getEndereco (char* palavra);

#endif

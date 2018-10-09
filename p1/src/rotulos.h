#ifndef ROTULOS_H
#define ROTULOS_H

typedef struct Rotulo{
    char nome[64];
    char endereco[4];
}Rotulo;

typedef struct tipoNome{
    char nome[64];
    char valor[64];
}tipoNome;

unsigned getNumberOfRotulos();

Rotulo getRotulo(int posicao);

void adicionarRotulo(Rotulo rotulo);

// Rotulo* criarRotulo(char* palavra, char* endereco);

char* getEndereco (char* palavra);

Rotulo recuperaRotulo(int posicao);



unsigned getNumberOfNomes();

tipoNome getNome(int posicao);

void adicionarNome(tipoNome nome);

// tipoNome* criarNome(char* palavra, char* valor);

char* getValor (char* nome);

tipoNome recuperaNome(int posicao);

#endif

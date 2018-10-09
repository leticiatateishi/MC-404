#ifndef ROTULOS_H
#define ROTULOS_H

typedef struct Rotulo{
    char* nome;
    char* endereco;
}Rotulo;

typedef struct tipoNome{
    char* nome;
    char* valor;
}tipoNome;

unsigned getNumberOfRotulos();

Rotulo getRotulo(int posicao);

void adicionarRotulo(Rotulo* rotulo);

Rotulo* criarRotulo(char* palavra, char* endereco);

char* getEndereco (char* palavra);


unsigned getNumberOfNomes();

tipoNome getNome(int posicao);

void adicionarNome(tipoNome* nome);

tipoNome* criarNome(char* palavra, char* valor);

char* getValor (char* nome);

#endif

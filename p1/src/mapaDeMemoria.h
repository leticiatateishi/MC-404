#include <stdlib.h>

#ifndef MAPADEMEMORIA_H
#define MAPADEMEMORIA_H

char mapaDeMemoria[4096];

/*  Funções referentes ao arquivo emitirMapaDeMemoria.c
 */
 void reescreverMapa();

 void removerDoisPontos (char* rotulo);


/*  Funções referentes ao arquivo escritaHexadecimal.c
 */
int posicaoMultiplaDe (int posicao, int multiplo, int posicaoBase);

int reescreverDiretiva (char* diretiva, char enderecoAtual[4], int* posicao, int *i, int verificarRotulos, int* linhasMapa);

char* reescreverHexadecimal (char* hexadecimal);

void reescreverInstrucao (char* instrucao, char* enderecoAtual, int* posicao, int* linhasMapa, int i);

void incrementarHexadecimal (char* hexadecimal);

int escreverEndereco (int i);

void minusculaParaMaiuscular (char* palavra);

#endif

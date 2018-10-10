#include <stdlib.h>

#ifndef MAPADEMEMORIA_H
#define MAPADEMEMORIA_H

char mapaDeMemoria[4096];

/*  Funções referentes ao arquivo emitirMapaDeMemoria.c
 */
 void removerDoisPontos (char* rotulo);

 void reescreverMapa();


/*  Funções referentes ao arquivo escritaHexadecimal.c
 */
 int posicaoMultiplaDe (int posicao, int multiplo);

int reescreverDiretiva (char* diretiva, char enderecoAtual[4], int* posicao, int *i, int verificarRotulos);

char* reescreverHexadecimal (char* hexadecimal);

char* reescreverInstrucao (char* instrucao);

// char* reescreverDecimal (char* decimal);

void incrementarHexadecimal (char* hexadecimal);

int pularLinha(int posicao);

#endif

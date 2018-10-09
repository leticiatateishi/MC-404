#include <stdlib.h>

#ifndef MAPADEMEMORIA_H
#define MAPADEMEMORIA_H

char mapaDeMemoria[4096];
/*  Funções referentes ao arquivo emitirMapaDeMemoria.c
 */
void escreverMapaDeMemoria (char* substring, int posicao);

// int posicaoPalavra (int posicao);

 int posicaoMultiplaDe (int posicao, int multiplo);

 char* removerDoisPontos (char* diretiva);


/*  Funções referentes ao arquivo escritaHexadecimal.c
 */
void reescreverDiretiva (char* diretiva, char enderecoAtual[4], int* posicao, int *i, int verificarRotulos);

char* reescreverHexadecimal (char* hexadecimal);

char* reescreverInstrucao (char* instrucao);

char* reescreverDecimal (char* decimal);

void incrementarHexadecimal (char* hexadecimal);

#endif

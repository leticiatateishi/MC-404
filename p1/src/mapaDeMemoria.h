#ifndef MAPADEMEMORIA_H
#define MAPADEMEMORIA_H

static char mapaDeMemoria[4096];
/*  Funções referentes ao arquivo emitirMapaDeMemoria.c
 */
void escreverMapaDeMemoria (char* substring, char* mapaDeMemoria, int posicao);

// int posicaoPalavra (int posicao);

 int posicaoMultiplaDe (int posicao, int multiplo);

 char* removerDoisPontos (char* diretiva);


/*  Funções referentes ao arquivo escritaHexadecimal.c
 */
char* reescreverDiretiva (char* diretiva, char enderecoAtual[4], int* posicao, int *i, int verificarRotulos);

char* reescreverHexadecimal (char* hexadecimal);

char* reescreverInstrucao (char* instrucao);

char* reescreverDecimal (char* decimal);

char* incrementarHexadecimal (char* hexadecimal);

#endif

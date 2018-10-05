#ifndef MAPADEMEMORIA_H
#define MAPADEMEMORIA_H

/*  Funções referentes ao arquivo emitirMapaDeMemoria.c
 */
void escreverMapaDeMemoria (char* substring, char* mapaDeMemoria, int posicao);

int posicaoPalavra (int posicao);


/*  Funções referentes ao arquivo escritaHexadecimal.c
 */
char* reescreverHexadecimal (char* hexadecimal);

char* reescreverInstrucao (char* instrucao);

char* reescreverDecimal (char* decimal);

char* incrementarHexadecimal (char* hexadecimal);

#endif

#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

/*  Funções referentes ao arquivo processarEntrada.c
 */
int processarEntrada(char*, unsigned tamanho);

Token criarToken(TipoDoToken t, char* p, unsigned l);

void adicionarAspas(char p[64], char novaPalavra[66]);

int verificarPalavra(char* palavra, int numero_linha);

void reescreverPalavra (char* palavra);

char maiusculaParaMinuscula(char letra);


/*  Funções referente ao arquvi definirTipoToken.c
 */
int definirTipoToken (char* palavra);

int eNome (char* palavra);

int eInstrucao (char *palavra);

int eDefinicaoRotulo (char* palavra);

int eAlfanumerico (char* palavra);

int eDiretiva (char* palavra);

int eHexadecimal(char* palavra);

int eDecimal(char letra);

int palavraEDecimal(char* palavra);

void removerAspas(char* palavra);


/*  Funções referentes ao arquivo verificarErrosGramaticais.c
 */
int verificarErrosGramaticais();

int tokenDiretiva (int posicao, Token atual);

int tokenRotulo (int posicao, Token atual);

int tokenInstrucao (int posicao, Token atual);

void removerAspas(char* palavra);

int decimalMaiorQue1023 (Token token);


#endif

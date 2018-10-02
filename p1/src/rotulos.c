#include <stdio.h>
#include "token.h"
#include "rotulos.h"

static Rotulo rotulos[4096];
static unsigned tamRotulos = 0;

unsigned getNumberOfRotulos() {
  return tamRotulos;
}

// void adicionarRotulo(){
//
// }

#include <stdio.h>

int funcao(int x){
    int y = 10*x;    // em MQ
    int k = y/2;     // em AC
    for (int i=0; i<9; i++){
        k = (k + (y/k))/2;
        // 107 := k
        // AC := MQ

        // MQ := AC / 107   DIV
        // AC := AC % 107

        // AC := MQ     LOAD
        // AC := AC + MEM[107]     ADD
        // AC := AC/2       RSH

    }
    return k;
}

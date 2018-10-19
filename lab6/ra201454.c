#include <stdio.h>

int main(){

    int n;
    printf("Digite um inteiro:\n");
    scanf("%d", &n);
    printf("Triangulo de Pascal com %d linhas.\n", n);
    int anterior[768];
    int atual[768];

    for (int linha = 0; linha < n; linha++){
        for (int coluna = 0; coluna <= linha; coluna++){
            if (coluna == 0 || coluna == linha)
                atual[coluna] = 1;
            else
                atual[coluna] = anterior[coluna-1] + anterior[coluna];
            printf("%d ", atual[coluna]);
        }
        for(int i = 0; i <= linha; i++)
            anterior[i] = atual[i];
        printf("\n");
    }

}

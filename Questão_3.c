//João Pedro de Morais Cunha - 241011161 e Matheus Eiki Kimura - 241025327 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Função para saber se um numero é primo*/
int ehPrimo(int a) {
    if (a <= 1) return 0; // não é

    for (int i = 2; i * i <= a; i++) {
        if (a % i == 0) return 0;
    }
    return 1;
}

/* Função tau usando fatores primos */
int funcaoTau(int N) {
    int total_divisores = 1;
    int div = N;

    for (int i = 2; i * i <= N; i++) {
        if (ehPrimo(i)) {
            int alpha = 0;
            while (div % i == 0) {
                alpha++;
                div /= i;
            }
            if (alpha > 0) {
                total_divisores *= (alpha + 1);
            }
        }
    }
    if (div > 1) {
        // div é primo de si mesmo
        total_divisores *= 2;
    }

    printf("Número total de divisores de %d: %d\n", N, total_divisores);
    return total_divisores;
}

/*Função que calcula a soma de todos os divisores de um número*/
int funcaoSigma(int N) {
    int soma_divisores = 0;
    for (int i = 1; i <= N; i++) {
        if (N % i == 0) {
            soma_divisores += i;
        }
    }
    printf("A soma de todos os divisores de %d é: %d\n", N, soma_divisores);
    return soma_divisores;
}

/* Função para calcular a Razão de Eficiência */
float Razao_de_Eficiencia(int N) {
    int tau = funcaoTau(N);
    int sigma = funcaoSigma(N);
    float eficiencia = (float)sigma / tau;
    printf("Valor da razão de eficiência para %d: %.2f\n", N, eficiencia);
    return eficiencia;
}

int main() {
    int numero = 0;
    puts("Codigo feito por:\n Matheus Eiki Kimura - 241025327\n Joao Pedro Morais Da Cunha - 241011161\n");
    printf("=========Calculadora de Razão de Eficiência=========\n");
    printf("Digite o valor do número que você quer encontrar a razão de eficiência: ");
    if (scanf("%d", &numero) != 1 || numero <= 1 || numero > 105) {
        printf("Erro. Valor inválido ou menor que 2.\n");
        return 1;
    }

    Razao_de_Eficiencia(numero);
    return 0;
}

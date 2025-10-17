// Questão 2: Chaves Periódicas (MMC)
#include <stdio.h>
#include <stdlib.h>

typedef long long lli;

// Algoritmo de Euclides para MDC
static lli gcd(lli a, lli b) {
    while (b != 0) {
        lli r = a % b;
        a = b;
        b = r;
    }
    return a;
}

// Mínimo Múltiplo Comum
static lli lcm(lli a, lli b) {
    if (a == 0 || b == 0) return 0;
    return (a / gcd(a, b)) * b;
}

void questao02_chaves_periodicas(void) {
    int N;
    int ciclos[10];
    const int LIMITE_ANO = 50;

    puts("==========================================================");
    puts("  QUESTAO 02: Chaves Periodicas (MMC)");
    printf("  Limite de Ano: %d\n", LIMITE_ANO);
    puts("==========================================================");

    printf("Insira o numero de chaves N (1 <= N <= 10): ");
    if (scanf("%d", &N) != 1 || N < 1 || N > 10) {
        puts("Entrada invalida para N. Finalizando.");
        return;
    }

    printf("Insira os %d ciclos de ativacao (2 <= C_i <= 20):\n", N);
    for (int i = 0; i < N; ++i) {
        if (scanf("%d", &ciclos[i]) != 1 || ciclos[i] < 2 || ciclos[i] > 20) {
            printf("Ciclo invalido C%d. Finalizando.\n", i + 1);
            return;
        }
    }

    lli mmc_total = ciclos[0];
    puts("\n[Calculo de Sincronizacao - MMC]");
    printf("Passo 0: MMC_Total = %lld\n", mmc_total);

    for (int i = 1; i < N; ++i) {
        lli ciclo_atual = ciclos[i];
        if (mmc_total % ciclo_atual != 0) mmc_total = lcm(mmc_total, ciclo_atual);
        printf("Passo %d: MMC(Atual, %lld) = %lld\n", i, ciclo_atual, mmc_total);
    }

    lli primeiro_ano_sincronizado = mmc_total;
    puts("\n==========================================================");
    if (primeiro_ano_sincronizado > LIMITE_ANO) {
        printf("Nao e possivel. O primeiro ano sincronizado (%lld) excede o limite de %d anos.\n",
               primeiro_ano_sincronizado, LIMITE_ANO);
    } else {
        puts("O primeiro ano futuro sincronizado (X > 0) e:");
        printf("X = %lld\n", primeiro_ano_sincronizado);
    }
    puts("==========================================================");
}

int main(void) {
    puts("Codigo feito por:\n Matheus Eiki Kimura - 241025327\n Joao Pedro Morais Da Cunha - 241011161\n");
    questao02_chaves_periodicas();
    return 0;
}
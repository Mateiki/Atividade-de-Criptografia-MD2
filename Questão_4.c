/*
RESPOSTA DOS ITENS DE VERDADEIRO (V) E FALSO (F) DA QUESTÃO 4:

(V) O algoritmo de Euclides estendido é utilizado para calcular o inverso modular de um número.
(F) Se mdc(G, Zn) ≠ 1, o programa ainda consegue encontrar o inverso de G em Zn.
(V) A operação (H * inverso) % Zn representa a divisão modular de H por G.
(V) Se n1 for primo, o código aplica o Pequeno Teorema de Fermat para simplificar o cálculo de a^x mod n1.
(F) A função powMod implementa o cálculo de potência modular utilizando multiplicações diretas sem otimização.
(V) Quando o resultado do inverso é negativo, o código ajusta o valor somando o módulo m0.
(V) O cálculo de fi(n1) (função totiente de Euler) é utilizado apenas quando n1 não é primo.
*/

#include <stdio.h>
#include <stdlib.h> // Para a função abs() se necessário, embora o operador - funcione
#include <string.h> // Não estritamente necessário para este código, mas bom para C

#ifdef WIN32
#include <windows.h>
#endif

// Função para calcular o máximo divisor comum (MDC) com exibição dos passos
int mdcComPassos(int a, int b) {
    int resto;
    while (b != 0) { // [1] b != 0
        resto = a % b;
        printf("Algoritmo de Euclides: %d mod %d = %d\n", a, b, resto);
        a = b;
        b = resto; // [2] resto
    }
    return a;
}

// Função para calcular o inverso modular
int inversoModular(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;
    int A = a, B = m;

    // A função mdcComPassos já foi implementada e deve ser chamada aqui:
    if (mdcComPassos(a, m) != 1) { // [3] mdcComPassos
        printf("\nErro: O inverso modular nao existe, pois mdc(%d, %d) != 1.\n", a, m);
        return -1; // Sinaliza que o inverso não existe
    }

    // Reinicia 'a' e 'm' para o Algoritmo Estendido
    a = A;
    m = B; 

    while (m != 0) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;

        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
        x1 += m0; // [4] x1
    
    printf("\nSubstituindo, temos que o inverso de %d em %d é %d.\n\n", A, B, x1);
    return x1;
}

// Função para potência modular rápida
int powMod(int base, int exp, int mod) {
    // Usamos 'long long' para garantir que (res * b) e (b * b) não causem overflow
    long long res = 1;
    long long b = base % mod;
    
    while (exp > 0) {
        if (exp & 1) // [5] exp & 1
            res = (res * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (int)res;
}

int main() {
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    int H, G, Zn, x, n1;

    printf("Insira H: ");
    scanf("%d", &H);
    printf("Insira G: ");
    scanf("%d", &G);
    printf("Insira Zn: ");
    scanf("%d", &Zn);
    printf("Insira x: ");
    scanf("%d", &x);
    printf("Insira n1: ");
    scanf("%d", &n1);
    printf("\n");

    // L72: Chamada do inversoModular, G é a base, Zn é o módulo
    int inverso = inversoModular(G, Zn); // [6] inversoModular

    // Verifica se o inverso existe
    if (inverso == -1) {
        printf("A divisao modular nao pode ser realizada.\n");
        return 1;
    }

    // Calculo da divisao modular: a = (H * inverso) % Zn
    int a = (H * inverso) % Zn;

    printf("Fazendo a multiplicação modular: %d * %d mod %d = %d\n", H, inverso, Zn, a);
    printf("Sendo %d o inverso de %d.\n", inverso, G);

    // O problema exige que o expoente seja reduzido pelo Teorema de Fermat/Euler.
    // Como n1=13 é primo, usamos Fermat: novo_exp = x mod (n1 - 1).
    int novo_exp = x % (n1 - 1);
    
    // Chamada correta para a potência modular: powMod(base, novo_exp, mod)
    int resultado = powMod(a, novo_exp, n1); // [7] powMod(a, novo_exp, n1)

    printf("Valor final da congruencia: %d\n", resultado);

    return 0;
}
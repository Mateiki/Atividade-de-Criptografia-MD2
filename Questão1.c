#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
// Cabeçalho do código com informações do aluno e da atividade
/*
    Alunos: Matheus Eiki Kimura, João Pedro Morais Da Cunha
    Matrícula: 241025327, 241011161​
    Disciplina: Matemática Discreta II
    Atividade: Questão 01
*/

typedef long long lli;

// --- Funções Auxiliares ---

// Função auxiliar para Pollard Rho: g(x) = (x^2 + 1) mod N
lli pollard_g(lli val, lli mod) {
    return ((val * val) % mod + 1) % mod;
}

// 1. Algoritmo de Euclides para MDC
lli gcd(lli a, lli b) {
    lli resto;
    printf("  [Euclides] Calculando mdc(%lld, %lld)\n", a, b);
    while (b != 0) {
        resto = a % b;
        printf("  [Euclides] %lld mod %lld = %lld\n", a, b, resto);
        a = b;
        b = resto;
    }
    return a;
}

// 2. Algoritmo Estendido de Euclides para Inverso Modular
lli modInverse(lli a, lli m) {
    lli m0 = m, t, q;
    lli x0 = 0, x1 = 1;
    lli a_copy = a;
    
    if (m == 1) return 0;
    
    printf("\n  [Euclides Estendido] Encontrando Inverso D para E=%lld mod z=%lld\n", a_copy, m0);
    
    // Calcula o MDC e os coeficientes x e y
    while (a > 1) {
        q = a / m;
        t = m;
        
        // Atualiza m e a (como no Euclides normal)
        m = a % m;
        a = t;
        
        // Atualiza x0 e x1
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    // Garante que o inverso seja positivo
    if (x1 < 0) {
        printf("  [Euclides Estendido] Inverso negativo: %lld. Ajustando: %lld + %lld\n", x1, x1, m0);
        x1 += m0;
    }
    printf("  [Euclides Estendido] O Inverso Modular D é: %lld\n", x1);
    return x1;
}

// Verifica se um número é primo (Simplificado, para verificar Fermat)
int is_prime(lli n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (lli i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Função auxiliar silenciosa para verificar MDC sem prints
lli gcd_silent(lli a, lli b) {
    while (b != 0) {
        lli resto = a % b;
        a = b;
        b = resto;
    }
    return a;
}

// Exponenciação Modular sem redução de expoente (corrigido)
lli power(lli base, lli exp, lli mod, lli z_n) {
    printf("\n  [Exponenciacao Modular] Calculando (%lld)^%lld mod %lld\n", base, exp, mod);

    lli res = 1;
    base %= mod;
    
    if (base == 0) return 0;

    // Loop de Exponenciação Rápida (Exponenciação Binária)
    while (exp > 0) {
        if (exp & 1) { // Verifica se o bit menos significativo é 1
            res = (res * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }

    return res;
}

// --- Funções Específicas da Questão 01 ---

// Etapa 1: Fatoração de Pollard Rho
lli pollard_rho(lli N) {
    printf("\n--- Etapa 1: Fatoracao de Pollard Rho para N=%lld ---\n", N);
    if (N <= 1 || is_prime(N)) {
        printf("  Nao e um composto valido para o teste.\n");
        return N;
    }
    if (N % 2 == 0) return 2; 

    lli x = 2; 
    lli y = 2; 
    lli d = 1;

    int iter_count = 0;
    int max_iterations = 100000; // Prevenir loop infinito
    
    while (iter_count < max_iterations) {
        iter_count++;
        x = pollard_g(x, N);
        y = pollard_g(pollard_g(y, N), N);
        
        lli diff = (x > y) ? (x - y) : (y - x);
        d = gcd(diff, N);

        printf("  [Pollard Rho - Iteracao %d] x=%lld, y=%lld, |x-y|=%lld, mdc=%lld\n", iter_count, x, y, diff, d);

        if (d == N) {
            printf("  [Pollard Rho] Falha temporaria. Tentando nova semente.\n");
            x = (rand() % (N - 2)) + 2; 
            y = x;
            d = 1;
            iter_count = 0;
        } else if (d != 1) {
            printf("  [Pollard Rho] Fator encontrado: %lld\n", d);
            return d;
        }
    }
    
    printf("  [Pollard Rho] ERRO: Numero maximo de iteracoes atingido.\n");
    return N;
}

// Fatoração recursiva para garantir primos
void factorar_primos(lli N, lli *p, lli *q) {
    if (is_prime(N)) {
        *p = N;
        *q = 1;
        return;
    }

    lli fator = pollard_rho(N);

    if (fator == N) {
        *p = N;
        *q = 1;
        return;
    }

    lli outro_fator = N / fator;

    if (is_prime(fator) && is_prime(outro_fator)) {
        *p = fator;
        *q = outro_fator;
        return;
    }

    if (!is_prime(fator)) {
        factorar_primos(fator, p, q);
    } else {
        *p = fator;
    }

    if (!is_prime(outro_fator)) {
        lli p2, q2;
        factorar_primos(outro_fator, &p2, &q2);
        if (*q == 1) *q = p2;
    } else {
        if (*q == 1) *q = outro_fator;
    }
}

// Mapeamento Letra -> Número (A=11, B=12, ..., Z=36, Espaço=00)
int char_to_num(char c) {
    if (c == ' ') return 0;
    if (c >= 'A' && c <= 'Z') return 11 + (c - 'A');
    return -1;
}

// Mapeamento Número -> Letra
char num_to_char(int n) {
    if (n == 0) return ' ';
    if (n >= 11 && n <= 36) return (char)('A' + (n - 11));
    return '?';
}

// Função Principal do Sistema RSA
void run_rsa_system() {
    lli N1, N2;
    lli p, q;
    lli n, z, E, D;
    char original_message[100];
    int numeric_blocks[100];
    lli encrypted_blocks[100];
    lli decrypted_blocks[100];
    int num_blocks = 0;

    printf("==========================================================\n");
    printf("  Projeto RSA com Pollard Rho e Teoremas Modulares (MD2)\n");
    printf("==========================================================\n");

    do {
        printf("Insira N1 (3 ou 4 digitos, produto de primos distintos): ");
        if (scanf("%lld", &N1) != 1) {
            printf("Entrada invalida!\n");
            while(getchar() != '\n');
            continue;
        }
        
        printf("Insira N2 (3 ou 4 digitos, produto de primos distintos, diferente de N1): ");
        if (scanf("%lld", &N2) != 1) {
            printf("Entrada invalida!\n");
            while(getchar() != '\n');
            continue;
        }

        if ((N1 < 100 || N1 > 9999) || (N2 < 100 || N2 > 9999) || N1 == N2) {
            printf("Erro: N1 e N2 devem ter 3 ou 4 digitos e ser distintos. Tente novamente.\n");
        } else {
            break;
        }
    } while (1);

    lli p1, q1, p2, q2;

    factorar_primos(N1, &p1, &q1);
    if (q1 == 1) {
        printf("\nERRO: N1 nao fatorado em dois primos distintos. Use um N1 que seja produto de primos distintos.\n");
        return;
    }

    factorar_primos(N2, &p2, &q2);
    if (q2 == 1) {
        printf("\nERRO: N2 nao fatorado em dois primos distintos. Use um N2 que seja produto de primos distintos.\n");
        return;
    }

    p = p1;
    q = p2;

    printf("\n==========================================================\n");
    printf("  Fatores Primos RSA Definidos\n");
    printf("==========================================================\n");
    printf("  Primo p (fator de N1): %lld\n", p);
    printf("  Primo q (fator de N2): %lld\n", q);

    n = p * q;
    z = (p - 1) * (q - 1);

    printf("\n--- Etapa 2: Geracao das Chaves RSA ---\n");
    printf("  Modulo n = p * q: %lld\n", n);
    printf("  Totiente z(n) = (p-1)*(q-1): %lld\n", z);

    printf("  [Escolha de E] Buscando o menor E > 1 tal que mdc(E, z) == 1.\n");
    for (E = 2; E < z; E++) {
        if (gcd(E, z) == 1) { 
            break;
        }
    }
    printf("  Expoente Publico E: %lld\n", E);

    D = modInverse(E, z); 

    lli verification = (E * D) % z;
    printf("\n  [Verificacao] E * D mod z(n) = (%lld * %lld) mod %lld = %lld\n", E, D, z, verification);
    if (verification != 1) {
        printf("  [ERRO] A relacao E * D mod z(n) = 1 nao foi satisfeita!\n");
    } 

    printf("\n==========================================================\n");
    printf("  Chaves Geradas\n");
    printf("==========================================================\n");
    printf("  Chave Publica (n, E): (%lld, %lld)\n", n, E);
    printf("  Chave Privada (n, D): (%lld, %lld)\n", n, D);

    printf("\n--- Etapa 3: Criptografia e Descriptografia ---\n");
    printf("Insira a mensagem (APENAS MAIUSCULAS e ESPACOS, max 99 chars): ");
    scanf(" %[^\n]", original_message);

    printf("\n  [Pre-Codificacao] Mensagem em blocos de 2 digitos (A=11, Espaco=00):\n");
    for (int i = 0; original_message[i] != '\0'; i++) {
        char c = original_message[i];
        int num = char_to_num(c);
        if (num != -1) {
            numeric_blocks[num_blocks++] = num;
            printf("    '%c' -> %02d\n", c, num);
        }
    }
    
    if (num_blocks == 0) {
        printf("Mensagem invalida. Finalizando.\n");
        return;
    }

    printf("\n  [Codificacao] C = M^E mod n (Passo a passo)\n");
    for (int i = 0; i < num_blocks; i++) {
        lli M = numeric_blocks[i];
        printf("\n  Bloco M=%lld. Criptografando...\n", M);
        lli C = power(M, E, n, z); 
        encrypted_blocks[i] = C;
        printf("  Bloco Cifrado C: %lld\n", C);
    }
   
    printf("\n  [Decodificacao] M = C^D mod n (Passo a passo)\n");
    for (int i = 0; i < num_blocks; i++) {
        lli C = encrypted_blocks[i];
        printf("\n  Bloco C=%lld. Descriptografando...\n", C);
        lli M = power(C, D, n, z); 
        decrypted_blocks[i] = M;
        printf("  Bloco Decifrado M: %lld\n", M);
        if (M > 36) {
            printf("  [AVISO] Valor decifrado (%lld) fora do range esperado (0-36)\n", M);
        }
    }

    char decrypted_message[100];
    printf("\n  [Reconversao] Blocos numericos para Texto Original:\n");
    for (int i = 0; i < num_blocks; i++) {
        decrypted_message[i] = num_to_char(decrypted_blocks[i]);
        printf("    %02lld -> '%c'\n", decrypted_blocks[i], decrypted_message[i]);
    }
    decrypted_message[num_blocks] = '\0';

    printf("\n==========================================================\n");
    printf("  Resultado Final\n");
    printf("==========================================================\n");
    printf("Mensagem Original: %s\n", original_message);
    printf("Mensagem Decifrada: %s\n", decrypted_message);

    if (strcmp(decrypted_message, original_message) == 0) {
        printf("CONFIRMADO: A mensagem decifrada e identica a original.\n");
    } else {
        printf("FALHA: A mensagem decifrada e diferente da original.\n");
    }
}

int main() {
    printf("Codigo feito por:\n Matheus Eiki Kimura - 241025327\n Joao Pedro Morais Da Cunha - 241011161\n\n");
    srand(time(NULL)); 
    run_rsa_system();
    return 0;
}

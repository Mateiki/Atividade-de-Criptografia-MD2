#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Cabeçalho do código com informações do aluno e da atividade
/*
    Alunos: Matheus Eiki Kimura, João Pedro Morais Da Cunha
    Matrícula: 241025327, 241011161​
    Disciplina: Matemática Discreta II
    Atividade: Questão 01
*/

typedef long long lli;

// --- Funções Auxiliares ---

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
    for (lli i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// 3. Exponenciação Modular Otimizada com Teoremas
lli power(lli base, lli exp, lli mod, lli z_n) {
    lli new_exp = exp;
    char* theorem_applied = "Teorema da Divisão Euclidiana (default)";

    printf("\n  [Exponenciacao Modular] Calculando (%lld)^%lld mod %lld\n", base, exp, mod);

    // 1. Pequeno Teorema de Fermat
    if (is_prime(mod)) {
        if (base % mod != 0) {
            new_exp = exp % (mod - 1);
            theorem_applied = "Pequeno Teorema de Fermat";
        }
    } 
    // 2. Teorema de Euler
    else if (gcd(base, mod) == 1) {
        new_exp = exp % z_n;
        theorem_applied = "Teorema de Euler (mdc(base, mod)=1)";
    }
    
    // Se nenhum dos anteriores se aplicar, ou for o caso geral (D/E),
    // a redução do expoente é feita via Divisão Euclidiana (modulo z_n)
    if (strcmp(theorem_applied, "Teorema da Divisão Euclidiana (default)") == 0 && mod != z_n) {
        new_exp = exp % z_n;
    }
    
    printf("  [Exponenciacao Modular] Teorema Aplicado: %s\n", theorem_applied); [cite: 64]
    
    if (new_exp != exp) {
        printf("  [Exponenciacao Modular] Expoente reduzido: %lld mod %lld = %lld\n", exp, (mod == z_n ? z_n : z_n), new_exp); [cite: 64]
    }

    // Exponenciação por Quadrados (Método eficiente)
    lli res = 1;
    base %= mod;
    
    if (base == 0) return 0;

    // Loop de Exponenciação Rápida
    while (new_exp > 0) {
        if (new_exp & 1) { // Verifica se o bit menos significativo é 1
            res = (res * base) % mod;
            // printf("    * Passo (bit 1): res = (%lld * %lld) mod %lld = %lld\n", res, base, mod, res);
        }
        base = (base * base) % mod;
        // printf("    * Passo (quadrado): base = (%lld * %lld) mod %lld = %lld\n", base, base, mod, base);
        new_exp >>= 1;
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

    // Semente x0 = 2
    lli x = 2; 
    lli y = 2; 
    lli d = 1;

    // Função de iteração: g(x) = (x^2 + 1) mod N
    lli g(lli val, lli mod) {
        return (val * val + 1) % mod;
    }

    int iter_count = 0;
    // Laço para encontrar o fator
    while (1) {
        iter_count++;
        // x1 avança um passo
        x = g(x, N);
        // x2 avança dois passos
        y = g(g(y, N), N);
        
        // Calculo de d = mdc(|x - y|, N)
        lli diff = abs(x - y);
        d = gcd(diff, N); // Usa a função gcd com passo a passo

        printf("  [Pollard Rho - Iteracao %d] x=%lld, y=%lld, |x-y|=%lld, mdc=%lld\n", iter_count, x, y, diff, d); [cite: 31]

        if (d == N) {
            // Se d=N, a iteração falhou ou o N é primo (improvável após verificação inicial).
            // Em implementações reais, recomeçaríamos com sementes diferentes.
            printf("  [Pollard Rho] Falha temporaria. Tentando nova semente.\n");
            x = rand() % (N - 2) + 2; 
            y = x;
            d = 1;
        } else if (d != 1) {
            printf("  [Pollard Rho] Fator encontrado: %lld\n", d);
            return d; // Fator não trivial
        }
    }
}

// Mapeamento Letra -> Número (A=11, B=12, ..., Z=36, Espaço=00)
int char_to_num(char c) {
    if (c == ' ') return 0; // Espaço = 00
    if (c >= 'A' && c <= 'Z') return 11 + (c - 'A');
    return -1; // Caractere não mapeado
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

    // Etapa 1.1: Entrada de dados e validação
    do {
        printf("Insira N1 (3 ou 4 digitos, produto de primos distintos): ");
        scanf("%lld", &N1);
        printf("Insira N2 (3 ou 4 digitos, produto de primos distintos, diferente de N1): ");
        scanf("%lld", &N2);

        if ((N1 < 100 || N1 > 9999) || (N2 < 100 || N2 > 9999) || N1 == N2) {
            printf("Erro: N1 e N2 devem ter 3 ou 4 digitos e ser distintos. Tente novamente.\n"); [cite: 25]
        } else {
            break;
        }
    } while (1);
    
    // Etapa 1.2: Fatoração
    lli p1 = pollard_rho(N1);
    lli p2 = pollard_rho(N2);

    // Definição dos primos RSA: p=fator de N1, q=fator de N2
    p = p1; 
    q = p2; 
    
    printf("\n==========================================================\n");
    printf("  Fatores Primos RSA Definidos\n");
    printf("==========================================================\n");
    printf("  Primo p (fator de N1): %lld\n", p); [cite: 35]
    printf("  Primo q (fator de N2): %lld\n", q); [cite: 35]
    
    // Etapa 2: Geração de Chaves RSA
    n = p * q; // Modulo RSA 
    z = (p - 1) * (q - 1); // Totiente de Euler z(n)

    printf("\n--- Etapa 2: Geracao das Chaves RSA ---\n");
    printf("  Modulo n = p * q: %lld\n", n);
    printf("  Totiente z(n) = (p-1)*(q-1): %lld\n", z);

    // Escolha do Expoente Público E
    printf("  [Escolha de E] Buscando o menor E > 1 tal que mdc(E, z) == 1.\n");
    for (E = 2; E < n; E++) {
        if (gcd(E, z) == 1) { 
            break;
        }
    }
    printf("  Expoente Publico E: %lld\n", E);

    // Cálculo do Expoente Privado D
    D = modInverse(E, z); 

    printf("\n==========================================================\n");
    printf("  Chaves Geradas\n");
    printf("==========================================================\n");
    printf("  Chave Publica (n, E): (%lld, %lld)\n", n, E); [cite: 45]
    printf("  Chave Privada (n, D): (%lld, %lld)\n", n, D); [cite: 46]

    // Etapa 3: Codificação e Decodificação
    printf("\n--- Etapa 3: Criptografia e Descriptografia ---\n");
    printf("Insira a mensagem (APENAS MAIUSCULAS e ESPACOS, max 99 chars): ");
    scanf(" %[^\n]", original_message);

    // 3.1 Pré-Codificação
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

    // 3.2 Codificação (Criptografia)
    printf("\n  [Codificacao] C = M^E mod n (Passo a passo)\n"); [cite: 53]
    for (int i = 0; i < num_blocks; i++) {
        lli M = numeric_blocks[i];
        printf("\n  Bloco M=%lld. Criptografando...\n", M);
        // Usa a função power com seleção de teorema
        lli C = power(M, E, n, z); 
        encrypted_blocks[i] = C;
        printf("  Bloco Cifrado C: %lld\n", C);
    }

    // 3.3 Decodificação (Descriptografia)
    printf("\n  [Decodificacao] M = C^D mod n (Passo a passo)\n"); [cite: 53]
    for (int i = 0; i < num_blocks; i++) {
        lli C = encrypted_blocks[i];
        printf("\n  Bloco C=%lld. Descriptografando...\n", C);
        // Usa a função power com seleção de teorema
        lli M = power(C, D, n, z); 
        decrypted_blocks[i] = M;
        printf("  Bloco Decifrado M: %lld\n", M);
    }

    // 3.4 Reconversão numérica em texto
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

    // 3.5 Confirmação
    if (strcmp(decrypted_message, original_message) == 0) {
        printf("CONFIRMADO: A mensagem decifrada e identica a original.\n");
    } else {
        printf("FALHA: A mensagem decifrada e diferente da original.\n");
    }
}

int main() {
    printf("Codigo feito por:\n Matheus Eiki Kimura - 241025327\n Joao Pedro Morais Da Cunha - 241011161\n");
    // Inicializa o gerador de números para a função rand() usada no Pollard Rho
    srand(time(NULL)); 
    run_rsa_system();
    return 0;
}
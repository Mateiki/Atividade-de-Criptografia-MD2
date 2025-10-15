/*João Pedro de Morais Cunha - 241011161*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <math.h>

// Definir um tamanho máximo de array razoável para a stack
#define MAX_MSG_LEN 10000

// -- Variáveis de Chaves Globais (Usadas para comunicação entre as funções) --
int p_a = 0;
int q_a = 0;
int e_rsa = 0;
int d_rsa = 0;

// -- Funções básicas -- //
int ehPrimo(int N){
    if (N <= 1) return 0;
    if (N <= 3) return 1;
    if (N % 2 == 0 || N % 3 == 0) return 0;
    
    for(int i = 5; i * i <= N; i = i + 6){
        if(N % i == 0 || N % (i + 2) == 0){
            return 0;
        }
    }
    return 1; 
}

int mdc (int a, int b){
    if (b == 0){
        return a;
    }
    return mdc(b, a%b);
}

int function(long long x, int N){
    long long int function_value = ((long long) x * x + 1)%N;
    return (int) function_value;
}
// ------------------------------- //

// -- Busca das chaves -- //
int Pollard_Rho(int N){
    int result = ehPrimo(N);
    if (N < 100 || result) { 
        return N;
    }
    
    int x = 2, y = 2, D = 1;

    while(D == 1){
        y = function(y, N);
        y = function(y, N);
        x = function(x, N);

        D = mdc(abs(y - x), N);
        
        if(D > 1 && D < N){
            return D;
        }

        if (D == N){
            // Reinicia a busca com novos pontos iniciais se necessário, 
            // mas aqui retornamos N para simplificar.
            return N; 
        }
    }
    return D;
}

int Non_trivial_factor_for_RSA(){
    int N1, N2;
    printf("Seja Bem-vindo a Questão 1. Nela, usaremos o método de pollard-rho\n");
    printf("Primeiro, digite o valor do 1° N (N1 - Número Composto): ");
    if (scanf("%d", &N1) != 1) return 0;

    printf("Segundo, digite o valor do 2° N (N2 - Número Composto): ");
    if (scanf("%d", &N2) != 1) return 0;
    
    if(N1 > 9999 || N2 > 9999 || N1 < 100 || N2 < 100){
        printf("Valores fora da faixa esperada (100 a 9999). Encerrando...\n");
        return 0;
    }

    int fator1 = Pollard_Rho(N1);
    int fator2 = Pollard_Rho(N2);

    printf("\nOk. Dito isso, o fator D esperado para N1 (%d) é: %d.\n", N1, fator1);
    printf("Ok. Dito isso, o fator D esperado para N2 (%d) é: %d.\n", N2, fator2);
    
    p_a = N1;
    q_a = N2;

    printf("\nValor de p é: %d.\n", p_a);
    printf("O valor de q é: %d.\n", q_a);
    // Para fins de geração de chaves, o usuário deve inserir p e q PRIMOS.
    // Vamos pedir p e q na Key_RSA para garantir que são primos.
    return 1;
}

int E(int z){
    int e;
    for(e = 3; e < z; e++){ // Começa em 3, pois e=2 é um caso a ser evitado em RSA fraco
        if(mdc(e, z) == 1){
            e_rsa = e;
            return e;
        }
    }
    return 0; // Falha em encontrar um expoente
}

int fatores_bezout(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = 1; 
        *y = 0; 
        return a;
    }

    int x1, y1;

    int d = fatores_bezout(b, a % b, &x1, &y1);

    *x = y1;
    *y = x1 - (a / b) * y1; 

    return d;
}

int inverso(int a, int b){
    int x;
    int y;

    printf("\nA sua equação para achar o inverso é: %d*x + %d*y = d, d = mdc(a, b)\n\n", a, b);

    int d = fatores_bezout(a, b, &x, &y);

    printf("O que foi achado: %d * (%d) + %d * (%d) = %d\n", a, x, b, y, d);

    int d_inverso = x;

    while (d_inverso < 0) {
        d_inverso += b;
    }
    d_rsa = d_inverso; // Armazena o valor global
    
    printf("Então %d é o inverso de %d mod %d.\n", d_inverso, a, b);
    
    return d_inverso;
}

// -- Criptografia -- //

long long exponenciacao_modular(long long base, int e, int N) {
    long long res = 1;
    base %= N;
    
    if (base == 0) return 0;
    
    while (e > 0) {
        if (e & 1) { 
            res = (res * base) % N;
        }
        e >>= 1; 
        base = (base * base) % N; 
    }
    return res;
}

void Criptografia(char *s, long long *output_array, int *tamanho_do_vetor) {
    int i = 0;
    while (*s != '\0' && i < MAX_MSG_LEN) {
        char current_char = toupper(*s);
        long long numerical_value = -1;

        if (current_char >= 'A' && current_char <= 'Z') {
            numerical_value = current_char - 'A' + 11; 
        } else if (current_char == ' ') {
            numerical_value = 0; 
        }
        
        if (numerical_value != -1) {
            output_array[i] = numerical_value;
            i++;
        }
        s++;
    }
    *tamanho_do_vetor = i;
}

// --- Funções de Execução --- 

int Key_RSA(){

    int n;
    n = p_a * q_a;
    int z;
    z = (p_a - 1) * (q_a - 1); // z = phi(n)
    
    int meu_e = E(z); // Encontra 'e'
    int D = inverso(meu_e, z); // Encontra 'd'

    printf("\n--- Chaves RSA Geradas ---\n");
    printf("Chave Pública: (%d, %d)\n", n, meu_e); // (n, e)
    printf("Chave Privada: (%d, %d)\n", n, D);    // (n, d)
    
    return n; 
}


void Criptografia_RSA(char *mensagem, int N, int E) {
    long long criptografada[MAX_MSG_LEN]; 
    int tamanho_array;

    Criptografia(mensagem, criptografada, &tamanho_array);

    printf("\n--- Criptografando ---\n");
    printf("Mensagem Original (Mapeada): ");
    for(int i = 0; i < tamanho_array; i++){
        printf("%lld ", criptografada[i]);
    }
    printf("\n");

    printf("Mensagem Cifrada (C = M^E mod N): ");
    for(int i = 0; i < tamanho_array; i++){
        long long C = exponenciacao_modular(criptografada[i], E, N);
        printf("%lld ", C);
        criptografada[i] = C; 
    }
    printf("\n");
}

void Descriptografia_RSA(long long *decompose, int tamanho_array, int N, int D) {
    
    printf("\n--- Descriptografia ---\n");
    
    printf("Mensagem Descriptografada (M = C^D mod N): ");
    for(int i = 0; i < tamanho_array; i++){
        long long M = exponenciacao_modular(decompose[i], D, N);
        
        if (M >= 11 && M <= 36) {
             printf("%c", (char)('A' + M - 11)); 
        } else if (M == 0) {
             printf(" "); 
        } else {
             printf("?"); 
        }
    }
    printf("\n");
}

int main(){
    char s[MAX_MSG_LEN]; 
    long long int array_cifrado[MAX_MSG_LEN]; 
    int N1, N2;

    printf("========================================================\n");
    printf("          ETAPA 1: Pollard-Rho\n");
    printf("========================================================\n");

    printf("Digite o valor de N1: ");
    scanf("%d", &N1);
    printf("\nDigite o valor de N2: ");
    scanf("%d", &N2);
    printf("\n");
    Pollard_Rho(N1);
    Pollard_Rho(N2); 

    p_a = N1;
    q_a = N2;

    printf("\n--> Primos RSA Selecionados: p = %lld, q = %lld\n", p_a, q_a);

    printf("\n========================================================\n");
    printf("          ETAPA 2: Geracao das Chaves RSA\n");
    printf("========================================================\n");
    
    // Chama a função que calcula N, Z(n), E e D.
    long long int N_rsa = p_a*q_a;
    
    printf("\n### Chaves RSA Geradas ###\n");
    printf("Chave Publica: (n = %lld, e = %lld)\n", N_rsa, e_rsa);
    printf("Chave Privada: (n = %lld, d = %lld)\n", N_rsa, d_rsa);

    printf("\n========================================================\n");
    printf("   ETAPA 3: Criptografia e Descriptografia (MENSAGEM)\n");
    printf("========================================================\n");
    
    int i = 0;
    char c; 
    
    printf("Digite a mensagem a ser criptografada: ");
    fflush(stdin); 

    while (i < (MAX_MSG_LEN - 1) && (c = getchar()) != '\n' && c != EOF) {
        s[i] = c; 
        i++;
    }
    s[i] = '\0'; 
    printf("Mensagem a ser usada: \"%s\"\n", s);
    Criptografia(s, &N_rsa, &e_rsa);
    
    // 3.2 Descriptografia (Decodificação)
    // A função fará o cálculo M = C^D mod N e a reconversão numérica em texto.
    Descriptografia_RSA(array_cifrado, i, N_rsa, d_rsa);
    
    return 0;
}
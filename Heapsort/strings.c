#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void heapify ( char **S , int32_t T , int32_t i );
void heapsort ( char **S , int32_t n );
void construir_heap ( char **S , int32_t n );

int32_t esquerdo ( int32_t i );
int32_t direito ( int32_t i );
void trocar ( char **a , char **b );
void trocar2 ( char **S , int32_t i , int32_t j );
int32_t hex_to_decimal(char *s);
char* decimal_to_hex(int32_t n);

typedef struct pack {
    char **S;
    int32_t size;
    bool read;
    
} pacote;

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    int32_t n, quant, printed = 0, cont=0;
    fscanf(input, "%d %d", &n, &quant);

    pacote *pacotes = malloc(n*sizeof(pacote));

    for (int i=0; i<n; i++) {
        pacotes[i].read = false;
    }

    for (int i=0; i<n; i++) {
        int pos, t;
        fscanf(input, "%d %d", &pos, &t);

        pacotes[pos].S = malloc(t*3*sizeof(char));
        pacotes[pos].size = t;

        for (int j=0; j<t; j++) {
            pacotes[pos].S[j] = malloc(3*sizeof(char));
            fscanf(input, "%s", pacotes[pos].S[j]);
        }

        pacotes[pos].read = true;

        if ((i+1)%quant == 0 || i==n-1) {
            if (pacotes[printed].read) {
                fprintf(output, "%d: ", cont++);

                while (pacotes[printed].read && printed < n) {
                    for (int j=0; j<pacotes[printed].size; j++) {
                        fprintf(output, "%s ", pacotes[printed].S[j]);
                    }
                    printed++;
                }
                if(printed != n) {
                    fprintf(output, "\n");
                }
            }
        }
    }
}

void heapify ( char **S , int32_t n , int32_t i ) {
    int32_t P = i, E = esquerdo(i), D = direito(i);
    if (E < n && hex_to_decimal(S[E]) > hex_to_decimal(S[P]))
        P = E;
    if (D < n && hex_to_decimal(S[D]) > hex_to_decimal(S[P]))
        P = D;
    if (P != i) {
        trocar2(S, P, i);
        heapify(S, n, P);
    }
}

void heapsort ( char **S , int32_t n ) {
    // Construção do heap máximo ou mínimo
    construir_heap (S , n);
    // Iterando sobre os nós do heap
    int32_t t = n-1;
    for ( int32_t i = n - 1; i > 0; i--) {
        // Trocando a raiz pelo i- ésimo
        trocar(& S [0] , & S [ i ]) ;
        // Aplicando heapify na raiz
        heapify (S , t-- , 0);
    }
}

void construir_heap ( char **S , int32_t n ) {
    for ( int32_t i = n / 2 - 1; i >= 0; i--)
        heapify (S , n , i ) ;
}

int32_t esquerdo ( int32_t i ) {
    return 2 * i + 1;
}

int32_t direito ( int32_t i ) {
    return 2 * i + 2;
}

void trocar ( char **a , char **b ) {
    char *aux = *a ;
    *a = *b ;
    *b = aux ;
}

void trocar2 ( char **S , int32_t i , int32_t j ) {
    char *aux = S[i] ;
    S [i] = S [j] ;
    S [j] = aux ;
}

int32_t hex_to_decimal(char *s) {
    char hex[3];
    strcpy(hex, s);

    int32_t decimal = 0;
    int32_t base = 1;
    int32_t length = strlen(hex);

    for (int32_t i = length - 1; i >= 0; i--) {
        if (hex[i] >= '0' && hex[i] <= '9') {
            decimal += (hex[i] - '0') * base;
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            decimal += (hex[i] - 'A' + 10) * base;
        }
        base*=16;
    }
    return decimal;
}

char* decimal_to_hex(int32_t n) {
    char hex[20];
    int32_t i = 0;
    while (n != 0) {
        int32_t temp = 0;
        temp = n % 16;
        if (temp < 10) {
            hex[i] = temp + 48;
            i++;
        } else {
            hex[i] = temp + 55;
            i++;
        }
        n = n/16;
    }

    char *hexa = malloc(20*sizeof(char));
    for (int32_t j=i-1; j>=0; j--) {
        hexa[i-j-1] = hex[j];
    }
    hexa[i] = '\0';

    return hexa;
}

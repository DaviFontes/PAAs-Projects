#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void hex_to_decimal(int32_t *V, char Vs[][3], int32_t n);
char* decimal_to_hex(int32_t n);

typedef struct pack {
    int32_t *V;
    int32_t size;
    bool sorted;
    
} pacote;

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    int32_t n, quant, printed = 0, cont=0;
    fscanf(input, "%d %d", &n, &quant);

    pacote *pacotes = malloc(n*sizeof(pacote));

    for (int i=0; i<n; i++) {
        pacotes[i].sorted = false;
    }

    for (int i=0; i<n; i++) {
        int pos, t;
        fscanf(input, "%d %d", &pos, &t);

        char Vs[t][3];
        pacotes[pos].V = malloc(t*sizeof(int32_t));
        pacotes[pos].size = t;

        for (int j=0; j<t; j++) {
            fscanf(input, "%s", Vs[j]);
        }

        hex_to_decimal(pacotes[pos].V, Vs, t);

        pacotes[pos].sorted = true;

        if ((i+1)%quant == 0 || i==n-1) {
            if (pacotes[printed].sorted) {
                fprintf(output, "%d: ", cont++);

                while (pacotes[printed].sorted && printed < n) {
                    for (int j=0; j<pacotes[printed].size; j++) {
                        if(pacotes[printed].V[j] < 16) {
                            fprintf(output, "0");
                        }
                        fprintf(output, "%s ", decimal_to_hex(pacotes[printed].V[j]));
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

void hex_to_decimal(int32_t *V, char Vs[][3], int32_t n) {
    for (int i=0; i<n; i++) {
        char hex[3];
        strcpy(hex, Vs[i]);

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

        V[i] = decimal;
    }
}

char* decimal_to_hex(int32_t n) {
    char hex[3];
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

    char *hexa = malloc(3*sizeof(char));
    for (int32_t j=i-1; j>=0; j--) {
        hexa[i-j-1] = hex[j];
    }
    hexa[i] = '\0';

    return hexa;
}

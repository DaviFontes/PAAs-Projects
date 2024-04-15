#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct pack {
    char **S;
    int32_t size;
    bool sorted;
} pacote;

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    int32_t n, quant, printed = 0, cont=0;
    fscanf(input, "%d %d", &n, &quant);

    pacote *pacotes = malloc(n*sizeof(pacote));

    for (int i=0; i<n; i++) pacotes[i].sorted = false;

    for (int i=0; i<n; i++) {
        int pos, t;
        fscanf(input, "%d %d", &pos, &t);

        pacotes[pos].S = malloc(t*sizeof(char*));
        pacotes[pos].size = t;

        for (int j=0; j<t; j++) {
            pacotes[pos].S[j] = malloc(3*sizeof(char));
            fscanf(input, "%s", pacotes[pos].S[j]);
        }

        pacotes[pos].sorted = true;

        if ((i+1)%quant == 0 || i==n-1) {
            if (pacotes[printed].sorted) {
                fprintf(output, "%d: ", cont++);
                while (pacotes[printed].sorted && printed < n) {
                    for (int j=0; j<pacotes[printed].size; j++) fprintf(output, "%s ", pacotes[printed].S[j]);
                    printed++;
                }
                if(printed != n) fprintf(output, "\n");
            }
        }
    }
}
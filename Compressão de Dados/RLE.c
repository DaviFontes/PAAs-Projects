#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

typedef struct texto {
    int tam;
    char **vetor;
    char *RLE;
    float taxaRLE;
} texto;

char* RLE(char **strings, int t);
char* tohex(int n);

void calcular_compressao(texto *text);

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int n;
    fscanf(input, "%d", &n);

    int cont=0;
    for (int i=0; i<n; i++) {
        texto *text = (texto*)malloc(sizeof(texto));

        fscanf(input, "%d", &text->tam);

        text->vetor = (char**)malloc(text->tam*sizeof(char*));
        
        for (int j=0; j<text->tam; j++) {
            text->vetor[j] = (char*)malloc(3*sizeof(char));
            fscanf(input, "%s", text->vetor[j]);
        }

        text->RLE = RLE(text->vetor, text->tam);

        calcular_compressao(text);

        if (strcmp(text->RLE, "false")==0) fprintf(output, "---\n");
        else {
            fprintf(output, "%d->RLE(%.2f%%)=%s\n", i, text->taxaRLE, text->RLE);
            cont++;
        }
    }
    printf("%d\n", cont);
    
}

char* RLE(char **strings, int t) {
    char *saida = (char*)malloc(t*sizeof(char));
    saida[0] = '\0';
    char *aux;
    aux = strings[0];
    int count = 1;
    
    for (int i=1; i<t; i++) {
        if (strlen(saida) > t*0.5) return "false";
        if (strcmp(strings[i], aux) == 0 && count < 255) {
            count++;
        } else {
            strcat(saida, tohex(count));
            strcat(saida, aux);
            aux = strings[i];
            count = 1;
        }
    }

    strcat(saida, tohex(count));
    strcat(saida, aux);

    return saida;
    
}

char* tohex(int n) {
    char *hex = (char*)malloc(3*sizeof(char));
    if (n/16 > 9) hex[0] = n/16 + 55;
    else hex[0] = n/16 + '0';
    
    if (n%16 > 9) hex[1] = (n%16) + 55;
    else hex[1] = (n%16) + '0';
    
    hex[2] = '\0';
    
    return hex;
}



void calcular_compressao(texto *text) {
    text->taxaRLE = (strlen(text->RLE)*100)/ (float) (2*text->tam);
}
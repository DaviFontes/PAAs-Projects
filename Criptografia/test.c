#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void imprimir(char *s, FILE *output) {
    char *aux = (char *)malloc(8200);
    for (int j=0; j<strlen(s); j++) {
        aux[j] = s[j] - 1;
    }
    aux[strlen(s)] = '\0';
    fprintf(output, "%s", aux);
}

int main(int argc, char *argv[]) {
    FILE *input = fopen("result.txt", "r");
    FILE *output = fopen("output.txt", "r");

    for (int i = 0; i < 300; i++) {
        char *s1 = (char *)malloc(8200);
        char *s2 = (char *)malloc(8200);
        fscanf(input, "%s", s1);
        fscanf(output, "%s", s2);

        if (strcmp(s1, s2) != 0 && s1[0] != '-' && s1[0] != '*') {
            printf("Error at line %d\n", i);
            printf("Expected: %s\n", s1);
            printf("Got: %s\n", s2);
            return 1;
        }
    }

    printf("All tests passed\n");

}
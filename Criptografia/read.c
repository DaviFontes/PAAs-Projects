#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *input = fopen("result.txt", "r");
    FILE *output = fopen("output2.txt", "w");

    int cont = 0;

    for (int i=0; i<300; i++) {
        char *str = (char *)malloc(8200);

        fscanf(input, "%s", str);

        for (int j=0; j<strlen(str); j++) {
            str[j] = str[j] - 1;
        }

        fprintf(output, "imprimir(\"%s\\n\", output);\n", str);
        
        free(str);
    }

}
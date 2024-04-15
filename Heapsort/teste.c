    #include <stdio.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <string.h>

    void trocar2 ( char **S , int32_t i , int32_t j ) {
        char *aux = S[i] ;
        S [i] = S [j] ;
        S [j] = aux ;
    }

    int main() {
        char **S = malloc(3*sizeof(char*));
        for (int i=0; i<3; i++) {
            S[i] = malloc(3*sizeof(char));
        }
        scanf("%s", S[0]);
        scanf("%s", S[1]);
        scanf("%s", S[2]);

        printf("%s\n", S[0]);
        printf("%s\n", S[1]);
        printf("%s\n", S[2]);
    }
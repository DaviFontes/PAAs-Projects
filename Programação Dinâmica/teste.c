#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    char **s = malloc(100*sizeof(char));
    char t[10] = "Hello\n";  
    for (int i=0; i<5; i++) s[i] = malloc(6*sizeof(char));
    s[0] = t;
    s[1]= "World\n";

    printf("%s\n", s[0]);
}
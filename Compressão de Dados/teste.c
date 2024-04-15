#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

char* toHex(int n) {
    char *hex = (char*)malloc(3*sizeof(char));
    if (n/16 > 9) hex[0] = n/16 + 55;
    else hex[0] = n/16 + '0';
    
    if (n%16 > 9) hex[1] = (n%16) + 55;
    else hex[1] = (n%16) + '0';
    
    hex[2] = '\0';
    

    return hex;
}



int main(){
    int x = 31;
    char *hex = toHex(x);

    char *s = (char*)malloc(3*sizeof(char));

    strcpy(s, hex);

}
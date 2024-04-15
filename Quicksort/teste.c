#include <stdio.h>
#include <stdint.h>

int32_t mediana(int32_t *V, int32_t i, int32_t j) {
    int32_t v1 = V[(j-i+1)/4], v2 = V[(j-i+1)/2], v3 = V[(j-i+1)*3/4];

    if(v1==v2 || v1==v3){
        return (j-i+1)/4;
    } else if(v2==v3){
        return (j-i+1)/2;
    } else if(v1<v2) {
        if(v1>v3) return (j-i+1)/4;
        else if (v2<v3) return (j-i+1)/2;
        else return (j-i+1)*3/4;
    } else {
        if(v2>v3) return (j-i+1)/2;
        else if (v1<v3) return (j-i+1)/4;
        else return (j-i+1)*3/4;
    }
}

int main() {
    int32_t v[10] = {103,12,1234,323,41445,1234,4426,1234,856,319};
    printf("%d\n", mediana(v, 0, 9));
    return 0;
}
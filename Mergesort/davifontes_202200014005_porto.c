// Padrão de tipos por tamanho
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct container {
    char codigo[12];
    char cnpj[19];
    int32_t peso;

    int32_t pos;

    int32_t erro;
    char cnpjAux[19];
    int32_t diffPerc;
    int32_t diffPeso;
} container;

void mergesort(container *S, container *E, int32_t i, int32_t j);
void intercalar(container *S, container *E, int32_t i, int32_t m, int32_t j);
void copiar(container *S, container *E, int32_t n);
int32_t busca_binaria(container *S, int32_t n, char *codigo);

void mergesort2(container *S, container *E, int32_t i, int32_t j);
void intercalar2(container *S, container *E, int32_t i, int32_t m, int32_t j);
void copiar2(container *S, container *E, int32_t n);

void mergesort3(container *S, container *E, int32_t i, int32_t j);
void intercalar3(container *S, container *E, int32_t i, int32_t m, int32_t j);

void mergesort4(container *S, container *E, int32_t i, int32_t j);
void intercalar4(container *S, container *E, int32_t i, int32_t m, int32_t j);

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int32_t n;
    fscanf(input, "%d", &n);
    
    container *containers = malloc(n*sizeof(container));
    container *containers_sort = malloc(n*sizeof(container));

    for (int i=0; i<n; i++) {
        fscanf(input, "%s %s %d", containers[i].codigo, containers[i].cnpj, &containers[i].peso);
        containers[i].pos = i;
    }

    mergesort(containers_sort, containers, 0, n-1);

    int32_t m;
    fscanf(input, "%d", &m);
    container *containers_aux = malloc(m*sizeof(container));
    container *containers_aux_sort = malloc(m*sizeof(container));
    
    int teste = 0;
    for (int i=0; i<m; i++) {
        fscanf(input, "%s %s %d", containers_aux[i].codigo, containers_aux[i].cnpj, &containers_aux[i].peso);
        int32_t index = busca_binaria(containers_sort, n, containers_aux[i].codigo);
        if (index != -1) {
            if(strcmp(containers_sort[index].cnpj, containers_aux[i].cnpj) != 0) {
                containers_aux[i].pos = containers_sort[index].pos;
                strcpy(containers_aux[i].cnpjAux, containers_sort[index].cnpj);
                containers_aux[i].erro = 1;
            }
            else if (((int) round(abs(containers_sort[index].peso - containers_aux[i].peso)*100/ (float) containers_sort[index].peso)) > 10) {
                containers_aux[i].pos = containers_sort[index].pos;
                containers_aux[i].erro = 2;
                containers_aux[i].diffPerc = (int) round(abs(containers_sort[index].peso - containers_aux[i].peso)*100/ (float) containers_sort[index].peso);
                containers_aux[i].diffPeso = abs(containers_sort[index].peso - containers_aux[i].peso);
            }
            else containers_aux[i].erro = 0;
        }
        else printf("%d\n", ++teste);
    }
    mergesort2(containers_aux_sort, containers_aux, 0, m-1);

    int32_t a=-1, b=0, c=0;

    while (containers_aux_sort[++a].erro == 0) b++;
    c=b;
    while (containers_aux_sort[++a].erro == 1) c++;

    container containers_cnpjs[c-b+1], containers_pesos[m-c-1], containers_cnpjs_sort[c-b+1], containers_pesos_sort[m-c-1];
    for (int i=0; i<c-b+1; i++) {
        strcpy(containers_cnpjs[i].codigo, containers_aux_sort[b+i].codigo);
        strcpy(containers_cnpjs[i].cnpj, containers_aux_sort[b+i].cnpj);
        containers_cnpjs[i].peso = containers_aux_sort[b+i].peso;
        containers_cnpjs[i].pos = containers_aux_sort[b+i].pos;
        containers_cnpjs[i].erro = containers_aux_sort[b+i].erro;
        strcpy(containers_cnpjs[i].cnpjAux, containers_aux_sort[b+i].cnpjAux);
        containers_cnpjs[i].diffPerc = containers_aux_sort[b+i].diffPerc;
        containers_cnpjs[i].diffPeso = containers_aux_sort[b+i].diffPeso;
    }
    for (int i=0; i<m-c-1; i++) {
        strcpy(containers_pesos[i].codigo, containers_aux_sort[c+1+i].codigo);
        strcpy(containers_pesos[i].cnpj, containers_aux_sort[c+1+i].cnpj);
        containers_pesos[i].peso = containers_aux_sort[c+1+i].peso;
        containers_pesos[i].pos = containers_aux_sort[c+1+i].pos;
        containers_pesos[i].erro = containers_aux_sort[c+1+i].erro;
        strcpy(containers_pesos[i].cnpjAux, containers_aux_sort[c+1+i].cnpjAux);
        containers_pesos[i].diffPerc = containers_aux_sort[c+1+i].diffPerc;
        containers_pesos[i].diffPeso = containers_aux_sort[c+1+i].diffPeso;
    }
    
    mergesort3(containers_cnpjs_sort, containers_cnpjs, 0, c-b);
    mergesort4(containers_pesos_sort, containers_pesos, 0, m-c-2);

    for (int i=0; i<c-b+1; i++){
        fprintf(output, "%s: %s<->%s\n", containers_cnpjs_sort[i].codigo, containers_cnpjs_sort[i].cnpjAux, containers_cnpjs_sort[i].cnpj);
    }

    for (int i=0; i<m-c-1; i++){
        if(i==m-c-2) fprintf(output, "%s: %dkg (%d%%)", containers_pesos_sort[i].codigo, containers_pesos_sort[i].diffPeso, containers_pesos_sort[i].diffPerc);
        else fprintf(output, "%s: %dkg (%d%%)\n", containers_pesos_sort[i].codigo, containers_pesos_sort[i].diffPeso, containers_pesos_sort[i].diffPerc);
    }

}   

void mergesort(container *S, container *E, int32_t i, int32_t j) {
    if (i < j) {
        int32_t m = i + (j - i) / 2;
        mergesort(S, E, i, m);
        mergesort(S, E, m + 1, j);
        intercalar(S, E, i, m, j);
    }
}

void intercalar(container *S, container *E, int32_t i, int32_t m, int32_t j){    
    int32_t i1 = i, i2 = m + 1, k = i;
    while (i1 <= m && i2 <= j) {
        if (strcmp(E[i1].codigo, E[i2].codigo)<=0) {
            strcpy(S[k].codigo, E[i1].codigo);
            strcpy(S[k].cnpj, E[i1].cnpj);
            S[k].peso = E[i1].peso;
            S[k++].pos = E[i1++].pos;
        }
        else {
            strcpy(S[k].codigo, E[i2].codigo);
            strcpy(S[k].cnpj, E[i2].cnpj);
            S[k].peso = E[i2].peso;
            S[k++].pos = E[i2++].pos;
        }
    }
    if (i1 > m) copiar(&S[k], &E[i2], j - i2 + 1);
    else copiar(&S[k], &E[i1], m - i1 + 1);
    copiar(&E[i], &S[i], j - i + 1);
}

void copiar(container *S, container *E, int32_t n) {
    for (int32_t i = 0; i < n; i++) {
        strcpy(S[i].codigo, E[i].codigo);
        strcpy(S[i].cnpj, E[i].cnpj);
        S[i].peso = E[i].peso;
        S[i].pos = E[i].pos;
    }
}

int32_t busca_binaria(container *S, int32_t n, char *codigo) {
    int32_t i = 0, j = n - 1, m;
    while (i <= j) {
        m = i + (j - i) / 2;
        if (strcmp(S[m].codigo, codigo) == 0) return m;
        else if (strcmp(S[m].codigo, codigo) < 0) i = m + 1;
        else j = m - 1;
    }
    return -1;
}

void mergesort2(container *S, container *E, int32_t i, int32_t j) {
    if (i < j) {
        int32_t m = i + (j - i) / 2;
        mergesort2(S, E, i, m);
        mergesort2(S, E, m + 1, j);
        intercalar2(S, E, i, m, j);
    }
}

void intercalar2(container *S, container *E, int32_t i, int32_t m, int32_t j){    
    int32_t i1 = i, i2 = m + 1, k = i;
    while (i1 <= m && i2 <= j) {
        if (E[i1].erro<E[i2].erro) {
            strcpy(S[k].codigo, E[i1].codigo);
            strcpy(S[k].cnpj, E[i1].cnpj);
            S[k].peso = E[i1].peso;
            strcpy(S[k].cnpjAux, E[i1].cnpjAux);
            S[k].diffPerc = E[i1].diffPerc;
            S[k].diffPeso = E[i1].diffPeso;
            S[k].erro = E[i1].erro;
            S[k++].pos = E[i1++].pos;
        }
        else {
            strcpy(S[k].codigo, E[i2].codigo);
            strcpy(S[k].cnpj, E[i2].cnpj);
            S[k].peso = E[i2].peso;
            strcpy(S[k].cnpjAux, E[i2].cnpjAux);
            S[k].diffPerc = E[i2].diffPerc;
            S[k].diffPeso = E[i2].diffPeso;
            S[k].erro = E[i2].erro;
            S[k++].pos = E[i2++].pos;
        }
    }
    if (i1 > m) copiar2(&S[k], &E[i2], j - i2 + 1);
    else copiar2(&S[k], &E[i1], m - i1 + 1);
    copiar2(&E[i], &S[i], j - i + 1);
}

void copiar2(container *S, container *E, int32_t n) {
    for (int32_t i = 0; i < n; i++) {
        strcpy(S[i].codigo, E[i].codigo);
        strcpy(S[i].cnpj, E[i].cnpj);
        S[i].peso = E[i].peso;
        strcpy(S[i].cnpjAux, E[i].cnpjAux);
        S[i].diffPerc = E[i].diffPerc;
        S[i].diffPeso = E[i].diffPeso;
        S[i].erro = E[i].erro;
        S[i].pos = E[i].pos;
    }
}

void mergesort3(container *S, container *E, int32_t i, int32_t j) {
    if (i < j) {
        int32_t m = i + (j - i) / 2;
        mergesort3(S, E, i, m);
        mergesort3(S, E, m + 1, j);
        intercalar3(S, E, i, m, j);
    }
}

void intercalar3(container *S, container *E, int32_t i, int32_t m, int32_t j){    
    int32_t i1 = i, i2 = m + 1, k = i;
    while (i1 <= m && i2 <= j) {
        if (E[i1].pos<E[i2].pos) {
            strcpy(S[k].codigo, E[i1].codigo);
            strcpy(S[k].cnpj, E[i1].cnpj);
            S[k].peso = E[i1].peso;
            strcpy(S[k].cnpjAux, E[i1].cnpjAux);
            S[k].diffPerc = E[i1].diffPerc;
            S[k].diffPeso = E[i1].diffPeso;
            S[k].erro = E[i1].erro;
            S[k++].pos = E[i1++].pos;
        }
        else {
            strcpy(S[k].codigo, E[i2].codigo);
            strcpy(S[k].cnpj, E[i2].cnpj);
            S[k].peso = E[i2].peso;
            strcpy(S[k].cnpjAux, E[i2].cnpjAux);
            S[k].diffPerc = E[i2].diffPerc;
            S[k].diffPeso = E[i2].diffPeso;
            S[k].erro = E[i2].erro;
            S[k++].pos = E[i2++].pos;
        }
    }
    if (i1 > m) copiar2(&S[k], &E[i2], j - i2 + 1);
    else copiar2(&S[k], &E[i1], m - i1 + 1);
    copiar2(&E[i], &S[i], j - i + 1);
}

void mergesort4(container *S, container *E, int32_t i, int32_t j) {
    if (i < j) {
        int32_t m = i + (j - i) / 2;
        mergesort4(S, E, i, m);
        mergesort4(S, E, m + 1, j);
        intercalar4(S, E, i, m, j);
    }
}

void intercalar4(container *S, container *E, int32_t i, int32_t m, int32_t j){    
    int32_t i1 = i, i2 = m + 1, k = i;
    while (i1 <= m && i2 <= j) {
        if (E[i1].diffPerc==E[i2].diffPerc) {
            if (E[i1].pos<E[i2].pos) {
                strcpy(S[k].codigo, E[i1].codigo);
                strcpy(S[k].cnpj, E[i1].cnpj);
                S[k].peso = E[i1].peso;
                strcpy(S[k].cnpjAux, E[i1].cnpjAux);
                S[k].diffPerc = E[i1].diffPerc;
                S[k].diffPeso = E[i1].diffPeso;
                S[k].erro = E[i1].erro;
                S[k++].pos = E[i1++].pos;
            }
            else {
                strcpy(S[k].codigo, E[i2].codigo);
                strcpy(S[k].cnpj, E[i2].cnpj);
                S[k].peso = E[i2].peso;
                strcpy(S[k].cnpjAux, E[i2].cnpjAux);
                S[k].diffPerc = E[i2].diffPerc;
                S[k].diffPeso = E[i2].diffPeso;
                S[k].erro = E[i2].erro;
                S[k++].pos = E[i2++].pos;
            }
        } else if (E[i1].diffPerc>E[i2].diffPerc) {
            strcpy(S[k].codigo, E[i1].codigo);
            strcpy(S[k].cnpj, E[i1].cnpj);
            S[k].peso = E[i1].peso;
            strcpy(S[k].cnpjAux, E[i1].cnpjAux);
            S[k].diffPerc = E[i1].diffPerc;
            S[k].diffPeso = E[i1].diffPeso;
            S[k].erro = E[i1].erro;
            S[k++].pos = E[i1++].pos;
        } else {
            strcpy(S[k].codigo, E[i2].codigo);
            strcpy(S[k].cnpj, E[i2].cnpj);
            S[k].peso = E[i2].peso;
            strcpy(S[k].cnpjAux, E[i2].cnpjAux);
            S[k].diffPerc = E[i2].diffPerc;
            S[k].diffPeso = E[i2].diffPeso;
            S[k].erro = E[i2].erro;
            S[k++].pos = E[i2++].pos;
        }
    }
    if (i1 > m) copiar2(&S[k], &E[i2], j - i2 + 1);
    else copiar2(&S[k], &E[i1], m - i1 + 1);
    copiar2(&E[i], &S[i], j - i + 1);
}
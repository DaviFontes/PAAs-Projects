#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct doenca {
    char codigo[9];
    int t;
    char **genes;
    int comp;
    int perc;
    int pos;
} doenca;

typedef struct resultado {
    char codigo[9];
    int perc;
    int pos;
} resultado;

void buscaKMP (int *k, int *R, char *T, char *P, int size, int pos);
void calcular_tabela (int *k, char *P);
void copiar(resultado *S, resultado *E, int n);
void mergesort(resultado *S, resultado *E, int i, int j);
void intercalar(resultado *S, resultado *E, int i, int m, int j);

int main(int argc, char **argv) {
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    int size;
    char dna[50000];
    fscanf(input, "%d %s", &size, dna);

    int quant;
    fscanf(input, "%d", &quant);
    doenca *doencas = malloc(quant*sizeof(doenca));
    resultado *resultadosIn = malloc(quant*sizeof(resultado));
    resultado *resultadosOut = malloc(quant*sizeof(resultado));

    for (int i=0; i<quant; i++) {
        fscanf(input, "%s %d", doencas[i].codigo, &doencas[i].t);
        doencas[i].genes = malloc(doencas[i].t*sizeof(char*));
        doencas[i].comp = 0;
        doencas[i].pos = i;

        for (int j=0; j<doencas[i].t; j++) {
            char s[1001];
            fscanf(input, "%s", s);
            doencas[i].genes[j] = malloc(sizeof(s));
            strcpy(doencas[i].genes[j], s);

            int *vetor = malloc(strlen(doencas[i].genes[j])*sizeof(int));
            int cont = 0;

            buscaKMP(vetor, &cont, dna, doencas[i].genes[j], size, 0);
            
            // printf("%d: %s\n", i, doencas[i].genes[j]);
            // printf("total: %d ", cont);
            // printf("\n\n");

            if(cont >= (float)strlen(doencas[i].genes[j])*9/10) {
                doencas[i].comp++;
            }
        }

        doencas[i].perc = (int) round((float) doencas[i].comp/doencas[i].t*100);
        strcpy(resultadosIn[i].codigo, doencas[i].codigo);
        resultadosIn[i].perc = doencas[i].perc;
        resultadosIn[i].pos = doencas[i].pos;
        
    }

    mergesort(resultadosOut, resultadosIn, 0, quant-1);

    for (int i=0; i<quant; i++) 
        fprintf(output, "%s->%d%%\n", resultadosOut[i].codigo, resultadosOut[i].perc);

} 

void buscaKMP (int *k, int *cont, char *T, char *P, int size, int pos) {
    // Pré - processamento
    int n = strlen(T), m = strlen(P), aux=0, sum = 0;
    calcular_tabela (k, P);
    // Iterando na cadeia T
    for (int i=pos, j=-1; i<n ; i++) {
        // Retorno de estado
        while (j>=0 && P[j+1] != T[i]) {
            if(j<size-1) {
                j=k[j];
                sum = j+1;
            }
            else if((strlen(P)-j-1)>=size) {
                char *sub = malloc((strlen(P)-j)*sizeof(char));
                int c=0;
                while (c<(strlen(P)-j)) {
                    sub[c] = P[j+1+c];
                    c++;
                }
                (*cont) += sum;
                buscaKMP(k, cont, T, sub, size, i);
                aux=1;
                break;
            } else {
                (*cont) += sum;
                aux=1;
                break;
            }
        }

        if(aux)break;

        if(P[j+1] == T[i]) {
            sum+=1;
            j++;
        }

        if(j == m-1) {
            (*cont) += sum;
            break;
        }
    }
}

// Procedimento de cálculo da tabela de transição
void calcular_tabela (int *k, char *P) {
    // i = sufixo , j = prefixo
    k[0] = -1;
    for (int i=1, j=-1; i<strlen(P); i++) {
        // Prefixo e sufixo diferentes
        while (j>=0 && P[j+1] != P[i])
        // Retorno de estado
        j = k[j];
        // Combinação de prefixo e sufixo
        if(P[j+1] == P[i])
        // Avanço de estado
        j++;
        // Atualização da transição do estado
        k[i] = j;
    }
}

void copiar(resultado *S, resultado *E, int n) {
    for (int i = 0; i < n; i++) {
        strcpy(S[i].codigo, E[i].codigo);
        S[i].perc = E[i].perc;
        S[i].pos = E[i].pos;
    }
}

void mergesort(resultado *S, resultado *E, int i, int j) {
    if (i < j) {
        int m = i + (j - i) / 2;
        mergesort(S, E, i, m);
        mergesort(S, E, m + 1, j);
        intercalar(S, E, i, m, j);
    }
}

void intercalar(resultado *S, resultado *E, int i, int m, int j){    
    int i1 = i, i2 = m + 1, k = i;
    while (i1 <= m && i2 <= j) {
        if (E[i1].perc==E[i2].perc) {
            if(E[i1].pos<E[i2].pos) {
                strcpy(S[k].codigo, E[i1].codigo);
                S[k].perc = E[i1].perc;
                S[k++].pos = E[i1++].pos;
            } else {
                strcpy(S[k].codigo, E[i2].codigo);
                S[k].perc = E[i2].perc;
                S[k++].pos = E[i2++].pos;
            }
        }
        else if (E[i1].perc>E[i2].perc) {
            strcpy(S[k].codigo, E[i1].codigo);
            S[k].perc = E[i1].perc;
            S[k++].pos = E[i1++].pos;
        } else {
            strcpy(S[k].codigo, E[i2].codigo);
            S[k].perc = E[i2].perc;
            S[k++].pos = E[i2++].pos;
        
        }
    }
    if (i1 > m) copiar(&S[k], &E[i2], j - i2 + 1);
    else copiar(&S[k], &E[i1], m - i1 + 1);
    copiar(&E[i], &S[i], j - i + 1);
}
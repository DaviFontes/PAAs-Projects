#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct carro {
    char placa[8];
    int32_t peso;
    int32_t volume;
}carro;

typedef struct produto{
    char codigo[14];
    float valor;
    int32_t peso;
    int32_t volume;
    int32_t usado;
    int32_t position;
}produto;

void tabela(float ***tabel, carro carro, produto *produtosAux, produto *produtos, int32_t p, FILE* output, int32_t *quant);
float max(float a, float b);
void removeUsedItems(float ***tabel, int32_t p, int32_t volume, int32_t peso, produto *produtosAux, produto *produtos, FILE* output, int32_t *quant);
void removeUsados(produto *produtos, produto *produtosAux, int32_t p);

float somaValor(produto *produtos, int32_t p);
int32_t somaPeso(produto *produtos, int32_t p);
int32_t somaVolume(produto *produtos, int32_t p);

int32_t main(int32_t argc, char* argv[]) {
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    int32_t c, p, quant = 0;
    fscanf(input, "%d", &c);
    carro *carros = malloc(c*sizeof(carro));
    

    for (int32_t i=0; i<c; i++) {
        fscanf(input, "%s %d %d", carros[i].placa, &carros[i].peso, &carros[i].volume);
    }


    fscanf(input, "%d", &p);
    produto *produtos = malloc(p*sizeof(produto));

    for (int32_t i=0; i<p; i++) {
        fscanf(input, "%s %f %d %d", produtos[i].codigo, &produtos[i].valor, &produtos[i].peso, &produtos[i].volume);
        produtos[i].usado = 0;
        produtos[i].position = i;
    }


    int32_t maxPeso = 0, maxVolume = 0;
    for (int32_t i=0; i<c; i++) {
        if (carros[i].peso > maxPeso) maxPeso = carros[i].peso;
        if (carros[i].volume > maxVolume) maxVolume = carros[i].volume;
    }

    float ***tabel;
    tabel = malloc((p+2)*sizeof(float**));
    for (int32_t i=0; i<=p; i++) {
        tabel[i] = malloc((maxVolume+2)*sizeof(float*));
        for (int32_t j=0; j<=maxVolume; j++) {
            tabel[i][j] = malloc((maxPeso+2)*sizeof(float));
        }
    }

    
    for (int32_t i=0;  i<c; i++) {
        produto *produtosAux = malloc((p-quant)*sizeof(produto));

        removeUsados(produtos, produtosAux, p);
        tabela(tabel, carros[i], produtosAux, produtos, p-quant, output, &quant);
        
        free(produtosAux);
    }

    //Pendente
    fprintf(output, "[PENDENTE]R$%.2f,%dKG,%dL\n", somaValor(produtos, p), somaPeso(produtos, p), somaVolume(produtos, p));
    for (int32_t i=0; i<p; i++) {
        if (produtos[i].usado == 0) {
            fprintf(output, "%s\n", produtos[i].codigo);
        }
    }
}
//checar linha 136
void tabela(float ***tabel, carro carro, produto *produtosAux, produto *produtos, int32_t p, FILE* output, int32_t *quant) {
    for (int32_t i=0; i<=p; i++) {
        for (int32_t j=0; j<=carro.volume; j++) {
            for (int32_t k=0; k<=carro.peso; k++) {
                if (i==0 || j==0 || k==0) {
                    tabel[i][j][k] = 0;
                } else if (produtosAux[i-1].volume <= j && produtosAux[i-1].peso <= k) {
                    tabel[i][j][k] = max(produtosAux[i-1].valor + tabel[i-1][j-produtosAux[i-1].volume][k-produtosAux[i-1].peso], tabel[i-1][j][k]);
                } else {
                    tabel[i][j][k] = tabel[i-1][j][k];
                }
            }
        }
    }

    
    // [AAA1234]R$100.00,49KG(98%),10L(10%)
    fprintf(output, "[%s]R$%.2f,", carro.placa, tabel[p][carro.volume][carro.peso]);
    
    removeUsedItems(tabel, p, carro.volume, carro.peso, produtosAux, produtos, output, quant);
   
}

float max(float a, float b) {
    return (a > b) ? a : b;
}

void removeUsedItems(float ***tabel, int32_t p, int32_t volume, int32_t peso, produto *produtosAux, produto *produtos, FILE* output, int32_t *quant) {
    int32_t i = p;
    int32_t j = volume;
    int32_t k = peso;
    int32_t aux = 0, pesototal = 0, volumetotal = 0;
    char **s = malloc(100*sizeof(char*));

    for (int32_t x=0; x<20; x++) {
        s[x] = malloc(14*sizeof(char));
    }
    while (i>0 && j>0 && k>0) {
        if (tabel[i][j][k] != tabel[i-1][j][k]) {
            s[aux++] = produtosAux[i-1].codigo;
            pesototal += produtosAux[i-1].peso;
            volumetotal += produtosAux[i-1].volume;
            produtos[produtosAux[i-1].position].usado = 1;
            j -= produtosAux[i-1].volume;
            k -= produtosAux[i-1].peso;
            (*quant)++;
        }
        i--;
    }
    fprintf(output, "%dKG(%.0f%%),%dL(%.0f%%)\n", pesototal, (float)pesototal/peso*100, volumetotal, (float)volumetotal/volume*100);
    
    for (int32_t x=aux-1; x>=0; x--) {
        fprintf(output, "%s\n", s[x]);
    }
}

void removeUsados(produto *produtos, produto *produtosAux, int32_t p) {
    int32_t j = 0;
    for (int32_t i=0; i<p; i++) {
        if (produtos[i].usado == 0) {
            produtosAux[j++] = produtos[i];
        }
    }
}

float somaValor(produto *produtos, int32_t p) {
    float soma = 0;
    for (int32_t i=0; i<p; i++) {
        if (produtos[i].usado == 0) {
            soma += produtos[i].valor;
        }
    }
    return soma;
}

int32_t somaPeso(produto *produtos, int32_t p) {
    int32_t soma = 0;
    for (int32_t i=0; i<p; i++) {
        if (produtos[i].usado == 0) {
            soma += produtos[i].peso;
        }
    }
    return soma;
}

int32_t somaVolume(produto *produtos, int32_t p) {
    int32_t soma = 0;
    for (int32_t i=0; i<p; i++) {
        if (produtos[i].usado == 0) {
            soma += produtos[i].volume;
        }
    }
    return soma;
}

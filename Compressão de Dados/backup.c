#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

//Só RLE funciona

typedef struct node {
    // Frequência
    int freq;
    // Código do símbolo
    char S;
    // Nó direito
    struct node *D;
    // Nó esquerdo
    struct node *E;
} no;

typedef struct fila_p_min {
    // Tamanho da fila
    int tam;
    // Capacidade da fila
    int cap;
    // Vetor de nós
    no **V;
} fila_p_min;

typedef struct texto {
    int tam;
    char **vetor;
    char *HUF;
    char *RLE;
    float taxaHUF;
    float taxaRLE;
} texto;


int hextodecimal (char H);
no * construir_arvore ( int H [] , int n );
fila_p_min * criar_fila_p_min ();
void inserir (fila_p_min *fpm, int freq, char S, no *E, no *D);
no * extrair_min (fila_p_min *fpm);
void tabela_codigos (no *A, char *T []);
void tabela_codigos_rec(no *A, char *C, int n, char *T[]);
char* RLE(char **strings, int t);
char* tohex(int n);
char* HUF(char **strings, int t);
void corrige_HUF(char **HUF, int t);
void bin_to_dec(char **bin);
void dec_to_hex(int dec, char **bin);
void calcular_compressao(texto *text);

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int n;
    fscanf(input, "%d", &n);
    for (int i=0; i<n; i++) {
        texto *text = (texto*)malloc(sizeof(texto));

        fscanf(input, "%d", &text->tam);

        text->vetor = (char**)malloc(text->tam*sizeof(char*));
        
        for (int j=0; j<text->tam; j++) {
            text->vetor[j] = (char*)malloc(3*sizeof(char));
            fscanf(input, "%s", text->vetor[j]);
        }

        text->RLE = RLE(text->vetor, text->tam);

        // text->HUF = HUF(text->vetor, text->tam); 
        // printf("HUF\n");
        // corrige_HUF(&text->HUF, text->tam);
        // printf("Corrigindo HUF\n");

        calcular_compressao(text);

        // if (text->taxaHUF < text->taxaRLE) {
        //     fprintf(output, "%d->HUF(%.2f%%)=%s\n", i, text->taxaHUF, text->HUF);
        // } else if (text->taxaHUF > text->taxaRLE) {
        //     fprintf(output, "%d->RLE(%.2f%%)=%s\n", i, text->taxaRLE, text->RLE);
        // } else {
        //     fprintf(output, "%d->HUF(%.2f%%)=%s\n", i, text->taxaHUF, text->HUF);
        //     fprintf(output, "%d->RLE(%.2f%%)=%s\n", i, text->taxaRLE, text->RLE);
        // } 
        if (text->taxaRLE > 25) fprintf(output, "%d->HUF\n", i);
        else fprintf(output, "%d->RLE(%.2f%%)=%s\n", i, text->taxaRLE, text->RLE);
    }
    
}

int hextodecimal (char H) {
    // Convertendo caractere hexadecimal para decimal
    if (H >= '0' && H <= '9') return H-'0';
    if (H >= 'A' && H <= 'F') return H-'A'+10;
    return 0;
}

no * construir_arvore ( int H [] , int n ) {
    // Criação de fila de prioridade mínima
    fila_p_min *fpm = criar_fila_p_min () ;
    // Inserindo símbolos não nulos na fila
    for (int i=0; i<n; i++)
        if(H[i]) inserir(fpm, H[i], i, NULL, NULL);

    // Combinação dos nós com menor frequência
    while (fpm->tam > 1) {
        no *x = extrair_min(fpm);
        no *y = extrair_min(fpm);
        inserir(fpm, x->freq+y->freq, '\0', x, y);
    }
    // Retornando a raiz da árvore
    return extrair_min(fpm);
}

fila_p_min * criar_fila_p_min () {
    // Alocando memória para a fila
    fila_p_min *fpm = (fila_p_min*)malloc(sizeof(fila_p_min));
    // Inicializando a fila
    fpm->tam = 0;
    fpm->cap = 16;
    fpm->V = (no**)malloc((fpm->cap+1)*sizeof(no*));
    // Retornando a fila
    return fpm;
}

void inserir (fila_p_min *fpm, int freq, char S, no *E, no *D) {
    // Aumentando o tamanho da fila
    fpm->tam++;
    // Inserindo nó na última posição
    fpm->V[fpm->tam] = (no*)malloc(sizeof(no));
    fpm->V[fpm->tam]->freq = freq;
    fpm->V[fpm->tam]->S = S;
    fpm->V[fpm->tam]->E = E;
    fpm->V[fpm->tam]->D = D;
    // Mantendo a propriedade de fila de prioridade mínima
    int i = fpm->tam;
    while (i > 1 && fpm->V[i]->freq < fpm->V[i/2]->freq) {
        no *aux = fpm->V[i];
        fpm->V[i] = fpm->V[i/2];
        fpm->V[i/2] = aux;
        i = i/2;
    }
}

no * extrair_min (fila_p_min *fpm) {
    // Armazenando nó raiz
    no *min = fpm->V[1];
    // Substituindo raiz pelo último nó
    fpm->V[1] = fpm->V[fpm->tam];
    // Diminuindo o tamanho da fila
    fpm->tam--;
    // Mantendo a propriedade de fila de prioridade mínima
    int i = 1;
    while (2*i <= fpm->tam) {
        int m = 2*i;
        if (2*i+1 <= fpm->tam && fpm->V[2*i+1]->freq < fpm->V[2*i]->freq) m = 2*i+1;
        if (fpm->V[i]->freq <= fpm->V[m]->freq) break;
        no *aux = fpm->V[i];
        fpm->V[i] = fpm->V[m];
        fpm->V[m] = aux;
        i = m;
    }
    // Retornando nó raiz
    return min;
}

void tabela_codigos (no *A, char *T []) {
    // Inicializando código
    char C[255];
    // Chamando função recursiva
    tabela_codigos_rec(A, C, 0, T);
}

void tabela_codigos_rec(no *A, char *C, int n, char *T []) {
    // Verificando se o nó é folha
    if (A->S) {
        // Anexando caractere nulo ao código
        C[n] = '\0';
        // Armazenando código na tabela
        int aux = A->S;
        strcpy(T[aux], C);
    } else {
        // Chamando função recursiva para o nó esquerdo
        C[n] = '0';
        tabela_codigos_rec(A->E, C, n+1, T);
        // Chamando função recursiva para o nó direito
        C[n] = '1';
        tabela_codigos_rec(A->D, C, n+1, T);
    }
}

char* RLE(char **strings, int t) {
    char *saida = (char*)malloc(t*4*sizeof(char));
    saida[0] = '\0';
    char *aux;
    aux = strings[0];
    int count = 1;
    
    for (int i=1; i<t; i++) {
        if (strlen(saida) > t*0.5) break;
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

char* HUF(char **strings, int t) {
    int *histograma = (int*)malloc(16*sizeof(int));

    for (int i=0; i<16; i++) histograma[i] = 0;

    for (int i=0; i<t; i++) {
        histograma[hextodecimal(strings[i][0])]++;
    }

    no *arvore = construir_arvore(histograma, 16);

    char *codigos[16];
    for (int i=0; i<16; i++) {
        codigos[i] = (char*)malloc(16*sizeof(char));
        codigos[i][0] = '\0';
    }

    tabela_codigos(arvore, codigos);

    char *saida = (char*)malloc(10000*t*sizeof(char));
    saida[0] = '\0';
    for (int i=0; i<t; i++) {
        strcat(saida, codigos[hextodecimal(strings[i][0])]);
    }

    return saida;
}

void corrige_HUF(char **HUF, int t) {
    int aux=0;
    if (strlen(*HUF) == 0) {
        *HUF = (char*)malloc(((t/8)+3)*sizeof(char));

        for (int i=0; i<t ; i++) if (i%4==0) (*HUF)[aux++] = '0';

        (*HUF)[aux] = '\0';
    } else {
        bin_to_dec(HUF);
    }
    if (strlen(*HUF)%2 != 0) {
        strcat(*HUF, "0");
    }
}

void bin_to_dec(char **bin){
    int dec = 0;
    int len = strlen(*bin);
    for(int i = 0; i < len; i++){
        if((*bin)[i] == '1'){
            dec += 1 << (len - i - 1);
        }
    }

    dec_to_hex(dec, bin);

}

void dec_to_hex(int dec, char **bin){
    char hex[100];
    int i = 0;
    while(dec != 0){
        int temp = 0;
        temp = dec % 16;
        if(temp < 10){
            hex[i] = temp + 48;
            i++;
        } else {
            hex[i] = temp + 55;
            i++;
        }
        dec = dec / 16;
    }
    hex[i] = '\0';

    int aux = 0;
    for(int j=i-1; j>=0 ; j--){
        (*bin)[aux++] = hex[j];
    }
    (*bin)[aux] = '\0';
    
}

void calcular_compressao(texto *text) {
    // text->taxaHUF = (strlen(text->HUF)*100)/ (float) (2*text->tam);
    text->taxaRLE = (strlen(text->RLE)*100)/ (float) (2*text->tam);
}
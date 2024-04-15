#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

/*Diffie-hellman
a = 256 bits
b = 256 bits
g = 2048 bits
p = 2048 bits
m = 32768 bits
c = 32768 bits
long long int = 64 bits
*/

typedef int64_t d_t;
typedef uint64_t s_t;   

#define base 10

typedef struct {
    uint64_t n, t;
    d_t *d;
} num_t;

//max
uint64_t max (uint64_t x, uint64_t y) {
    return x > y ? x : y;
}

//ajustar_n
void ajustar_n (num_t *x) {
    while (x->n > 0 && x->d[x->n - 1] == 0) x->n--;
}


// Procedimento de adição
void adicionar ( num_t * w , num_t * u , num_t * v ) {
    // Inicialização de variáveis
    s_t c = 0; d_t wi = 0;
    w->n = max(u->n, v->n);
    // wi = ui + vi + c
    for ( uint64_t i = 0; i < w->n ; i ++) {
        wi = u->d[i] + v->d[i] + c;
        w->d[i] = wi % base;
        c = wi / base;
    }
    // Ajuste do carry e dígitos
    if(w->n < w->t) w->d[w->n++] = c;
            
    ajustar_n (w) ;
}


void subtrair(num_t *w, num_t *u, num_t *v) {
    // Inicialização de variáveis
    s_t c = 0; d_t wi = 0;
    w->n = max(u->n, v->n);
    while (v->n < w->n) v->d[v->n++] = 0;
    // wi = ui - vi + c
    for (uint64_t i = 0; i < w->n; i++) {
        // printf("c: %d\n", c);
        wi = (u->d[i] - v->d[i] + c);
        // printf("wi: %d\n", wi);

        if (wi < 0) {
            wi += base;
            c = -1;
        }
        else c = 0;
        
        w->d[i] = wi % base;
    }
    // Ajuste do carry ( extensão de sinal ) e dígitos
    while (c != 0 && w->n < w->t) w->d[w->n++] = c;
    ajustar_n(w);
}

//criar
num_t *criar() {
    num_t *x = (num_t *)malloc(sizeof(num_t));
    x->n = 0;
    x->t = 100;
    x->d = (d_t*)calloc(x->t, sizeof(d_t));
    return x;
}

//min
uint64_t min (uint64_t x, uint64_t y) {
    return x < y ? x : y;
}

//atribuir
void atribuir (num_t *w, num_t *x) {
    w->n = x->n;
    for (uint64_t i = 0; i < x->n; i++) {
        w->d[i] = x->d[i];
    }
}

void atribuir_digito(num_t *x, d_t v) {
    x->n = 1;
    x->d[0] = v;
}

//destruir
void destruir(num_t **x) {
    free((*x)->d);
    free(*x);
    *x = NULL;
}

void multiplicar (num_t *w, num_t *u, num_t *v) {
    // Inicialização de variáveis
    s_t c = 0; d_t wij = 0; num_t *x = criar();
    x->n = min(u->n + v->n, x->t);
    // wi+j = wi+j + ui * vj + c
    for (uint32_t i = 0, j; i < u->n; c = 0, i++) {
        for (j = 0; j < v->n; j++) {
            wij = x->d[i+j] + u->d[i] * v->d[j] + c;
            x->d[i+j] = wij % base;
            c = wij / base;
        }
        if(i + j < x->t) x->d[i+j] = c;
    }

    ajustar_n(x);
    atribuir(w, x);
    destruir(&x);
}

// Procedimento para checagem de estimativa
void checar_estimativa(d_t *qc, d_t *rc, num_t *x, num_t *y, int32_t i) {
    // qc = b or qc * y[m - 2] > rc * b + x[m - 2 + i]
    if ((*qc) == base || (*qc )*y->d[y->n-2] > (*rc) * base + x->d[y->n-2+i]) {
        // Ajustando quociente e resto
        (*qc) = (*qc) - 1;
        (*rc) = (*rc) + y->d[y->n-1];
    }
}
// Procedimento de ajuste de estimativas
void ajustar_qc_rc(d_t *qc, d_t *rc, num_t *x, num_t *y, int32_t i) {
    // Checando estimativa
    checar_estimativa(qc, rc, x, y, i);
    if ((*rc) < base) checar_estimativa(qc, rc, x, y, i);
}

//deslocar_esquerda
void deslocar_esquerda(num_t *x, int32_t i) {
    for (int32_t j = x->n - 1; j >= 0; j--) {
        x->d[j + i] = x->d[j];
    }
    for (int32_t j = 0; j < i; j++) {
        x->d[j] = 0;
    }
    x->n += i;
}

// Procedimento de checagem de overflow
void checar_overflow(num_t *q, num_t *x, num_t *y, int32_t i) {
    // Checagem de overflow da subtração
    if(x->d[y->n+1+i] != 0) {
        // Reduzir quociente
        q->d[i] = q->d[i] - 1;
        // Adicionar divisor no dividendo
        y->d[y->n] = 0;
        deslocar_esquerda(y, i);
        adicionar(x, x, y);
    }
}

//multiplicar_digito
void multiplicar_digito(num_t *w, num_t *u, d_t v) {
    // Inicialização de variáveis
    num_t *x = criar();
    s_t c = 0; d_t wi = 0;
    x->n = u->n + 1;
    // wi = wi + ui * v + c
    for (uint64_t i = 0; i < u->n && v != 0; c=0, i++) {
        wi = x->d[i] + u->d[i] * v + c;
        x->d[i] = wi % base;
        c = wi / base;
        x->d[i+1] = c;
    }

    ajustar_n(x);
    atribuir(w, x);
    destruir(&x);
}

//zerar
void zerar(num_t *x) {
    x->n = 0;
    for (uint64_t i = 0; i < x->t; i++) {
        x->d[i] = 0;
    }
}

// Procedimento de divisão por digito
d_t dividir_digito(num_t* q, num_t* u, s_t v) {
    // Resto da divisão
    d_t r = 0;
    // Checando se dígito é zero
    if(v > 0) {
        // Variáveis auxiliares
        num_t* x = criar();
        s_t c = 0;
        d_t t = 0;
        // Quantidade de digitos de x
        x->n = u->n;
        // Iterando sobre os dígitos de u
        for(int32_t i = u->n - 1; i >= 0; i--) {
        // Calculando dividendo, quociente e carry
        t = c * base + u->d[i];
        x->d[i] = t / v;
        c = t % v;
        }
        // Obtendo o resto
        r = c;
        // Ajuste na quantidade de dígitos de x
        ajustar_n(x);
        // Atribuindo xa q
        atribuir(q, x);
        // Desalocando número auxiliar
        destruir(&x);
    }
    // Divisão por zero
    else {
        // Exibindo mensagem de erro
        printf("dividir_digito: divisão por zerol\n");
    }
    return r;
}

void dividir_numero(num_t *q, num_t *r, num_t *u, num_t *v) {
    // Criando números auxiliares
    num_t *x = criar();
    num_t *y = criar();
    // Normalização dos números
    s_t f = base / (v->d[v->n - 1] + 1);
    multiplicar_digito(x, u, f);
    multiplicar_digito(y, v, f);
    // Inicializando q
    zerar(q);
    // Quantidade de dígitos de q
    q->n = u->n - v->n + 1;

    for (int32_t i = q->n - 1; i >= 0; i--) {
        // Calculando e ajustando as estimativas
        d_t xx = x->d[v->n + i] * base + x->d[v->n - 1 + i];
        d_t qc = xx / y->d[v->n - 1], rc = xx % y->d[v->n - 1];
        ajustar_qc_rc(&qc, &rc, x, y, i);
        // Realizando a divisão
        multiplicar_digito(r, y, qc);
        deslocar_esquerda(r, i);
        subtrair(x, x, r);               
        q->d[i] = qc;
        checar_overflow(q, x, y, i);
    }
    
    // Calculando o resto
    dividir_digito(r, x, f);
    // Ajuste na quantidade de dígitos de q
    ajustar_n(q);    
    // Desalocando números auxiliares
    destruir(&x);
    destruir(&y);
}

// Procedimento de comparação
int32_t comparar(num_t *u, num_t *v) {
    // Inicialização de variáveis
    if (u->n < v->n) return 1;
    else if (u->n > v->n) return 0;
    else {
        int64_t i = u->n - 1;
        // Comparação de dígitos
        while(i >= 0 && u->d[i] == v->d[i]) i--;
        // Retorno da comparação
        return (i < 0) ? 0 : (u->d[i] < v->d[i]) ? 1 : 0;
    }
}

// Procedimento de divisão
void dividir(num_t *q, num_t *r, num_t *u, num_t *v) {
    // Divisor com apenas 1 dígito
    if(v->n == 1) {
        // q = u / v[0] , r = u % v[0]
        zerar(r);
        r->d[0] = dividir_digito(q, u, v->d[0]);
        r->n = (r->d[0] != 0);
    }
    // Divisor é maior do que dividendo
    else if(comparar(u, v)) {
        // q = 0 , r = u
        zerar(q);
        atribuir(r, u);
    }
    // Divisão longa dos números
    else dividir_numero(q, r, u, v);
}

// Procedimento de exponenciação
void exponenciar(num_t *v, num_t *u, num_t *k) {
    // x = u, y = k, v = 1
    num_t *x = criar(); num_t *y = criar();
    atribuir(x, u); atribuir(y, k); atribuir_digito(v, 1);
    // Repete enquanto y > 0
    while (y->n > 0) {
        if(dividir_digito(y, y, 2) == 1){
            multiplicar(v, v, x);
            dividir_numero(v, y, v, k);
        }
        multiplicar(x, x, x);
        dividir_numero(x, y, x, k);
    }
    destruir(&x); destruir(&y);
}

//Diffie-hellman
void diffie_hellman(num_t *a, num_t *b, num_t *g, num_t *p, FILE *output) {
    // Inicialização de variáveis
    num_t *aux = criar();
    num_t *aux2 = criar();
    num_t *s = criar();
    num_t *r = criar();
    // Exponenciação de g^a
    // exponenciar(aux, g, a);
    // Exponenciação de aux^b
    // exponenciar(aux2, aux, b);
    // Mod de aux2 por p
    // dividir(s, r, aux2, p);

    dividir(s, r, a, b);
    
    fprintf(output, "s: ");
    for (int i = 0; i < s->n; i++) {
        fprintf(output, "%d", s->d[i]);
    }
    fprintf(output, "\n");

    destruir(&aux); destruir(&aux2); destruir(&s); destruir(&r);
}



int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int n;
    fscanf(input, "%d", &n);
    
    for (int i = 0; i < n; i++) {

        char *op = (char *)malloc(3*sizeof(char));
        fscanf(input, "%s", op);

        if (strlen(op)==2) {
            char *a = (char *)malloc(65*sizeof(char));
            char *b = (char *)malloc(65*sizeof(char));
            char *g = (char *)malloc(513*sizeof(char));
            char *p = (char *)malloc(513*sizeof(char));    
            fscanf(input, "%s %s %s %s", a, b, g, p);

            num_t *na = criar();
            num_t *nb = criar();
            num_t *ng = criar();
            num_t *np = criar();

            for (int i = strlen(a) - 1; i >= 0; i--) {
                na->d[na->n++] = a[i] - '0' < 17? a[i] - '0' : a[i] - '0' - 7;
            }

            for (int i = strlen(b) - 1; i >= 0; i--) {
                nb->d[nb->n++] = b[i] - '0' < 17? b[i] - '0' : b[i] - '0' - 7;
            }

            for (int i = strlen(g) - 1; i >= 0; i--) {
                ng->d[ng->n++] = g[i] - '0' < 17? g[i] - '0' : g[i] - '0' - 7;
            }

            for (int i = strlen(p) - 1; i >= 0; i--) {
                np->d[np->n++] = p[i] - '0' < 17? p[i] - '0' : p[i] - '0' - 7;
            }

            diffie_hellman(na, nb, ng, np, output);
            
        } else if (op[0]=='d') {
            char *c = (char *)malloc(8193*sizeof(char));
            fscanf(input, "%s", c);

            free(c);
        } else {
            char *m = (char *)malloc(8193*sizeof(char));
            fscanf(input, "%s", m);

            free(m);
        }

        free(op);
    }

}
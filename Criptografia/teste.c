#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
        // printf("w->d[i]: %d\n", w->d[i]);
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
    s_t c = 0; d_t wij = 0;
    w->n = u->n;
    // wi = ui * v + c
    for (uint64_t i = 0; i < w->n; i++) {
        wij = u->d[i] * v + c;
        w->d[i] = wij % base;
        c = wij / base;
    }
    // Ajuste do carry e dígitos
    if (c != 0 && w->n < w->t) w->d[w->n++] = c;
    ajustar_n(w);
}

//zerar
void zerar(num_t *x) {
    x->n = 0;
    for (uint64_t i = 0; i < x->t; i++) {
        x->d[i] = 0;
    }
}

// Procedimento de divisão de dígitos
d_t dividir_digito(num_t *q, num_t *u, d_t v) {
    // Inicialização de variáveis
    s_t c = 0; d_t t = 0;
    q->n = u->n;
    // t = ui + c * b
    for (uint64_t i = q->n - 1; i >= 0; i--) {
        t = u->d[i] + c * base;
        q->d[i] = t / v;
        c = t % v;
    }
    // Ajuste do carry
    return c;
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
    else if(v->n > u->n) {
        // q = 0 , r = u
        zerar(q);
        atribuir(r, u);
    }
    // Divisão longa dos números
    else dividir_numero(q, r, u, v);
}


int main() {
    int x4 = 8, x3 = 1, x2 = 3, x1 = 2;
    int y3 = 4, y2 = 4, y1 = 3;

    num_t *w = (num_t *)malloc(sizeof(num_t));
    num_t *u = (num_t *)malloc(sizeof(num_t));
    num_t *v = (num_t *)malloc(sizeof(num_t));
    num_t *r = (num_t *)malloc(sizeof(num_t));

    r->n = 0;
    r->t = 100;
    r->d = (d_t*)malloc(r->t * sizeof(d_t));

    w->n = 0;
    w->t = 100;
    w->d = (d_t*)malloc(w->t * sizeof(d_t));

    u->n = 0;
    u->t = 100; 
    u->d = (d_t*)malloc(u->t * sizeof(d_t));

    v->n = 0;
    v->t = 100;
    v->d = (d_t*)malloc(v->t * sizeof(d_t));

    u->d[0] = x1;
    u->d[1] = x2;
    u->d[2] = x3;
    u->d[3] = x4;
    u->n = 4;

    v->d[0] = y1;
    v->d[1] = y2;
    v->d[2] = y3;
    v->n = 3;

    dividir(w, r, u, v);

    printf("Resultado: %d\n", 8132 / 443);
    printf("Quociente: %d\n", 8132 % 443);

    for (int i = w->n - 1; i>=0; i--) {
        printf("%d ", w->d[i]);
    }
    printf("\n");
    printf("r: ");
    for (int i = r->n - 1; i>=0; i--) {
        printf("%d ", r->d[i]);
    }
}
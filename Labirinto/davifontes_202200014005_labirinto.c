#include <stdio.h>
#include <stdlib.h>

void addBacktracking(int **backtracking, int coords[2], int *index) {
    backtracking[*index] = (int *)malloc(2*sizeof(int));
    backtracking[*index][0] = coords[0];
    backtracking[*index][1] = coords[1];
    (*index)++;
}

int beco(char **labirinto, int coords[2]) {
    if (labirinto[coords[0]][coords[1]+1] == '1' && labirinto[coords[0]-1][coords[1]] == '1' && labirinto[coords[0]][coords[1]-1] == '1' && labirinto[coords[0]+1][coords[1]] == '1')
        return 1;
    return 0;
}

int borda(int coords[2], int colunas, int linhas) {
    if (coords[0] == 0 || coords[0] == linhas-1 || coords[1] == 0 || coords[1] == colunas-1)
        return 1;
    return 0;
}

void goRight(int coords[2], FILE *output) {
    fprintf(output, "D@%d,%d->%d,%d\n", coords[0], coords[1], coords[0], coords[1]+1);
    coords[1]++;
}

void goUp(int coords[2], FILE *output) {
    fprintf(output, "F@%d,%d->%d,%d\n", coords[0], coords[1], coords[0]-1, coords[1]);
    coords[0]--;
}

void goLeft(int coords[2], FILE *output) {
    fprintf(output, "E@%d,%d->%d,%d\n", coords[0], coords[1], coords[0], coords[1]-1);
    coords[1]--;
}

void goDown(int coords[2], FILE *output) {
    fprintf(output, "T@%d,%d->%d,%d\n", coords[0], coords[1], coords[0]+1, coords[1]);
    coords[0]++;
}

void backtrack(int **backtracking, int coords[2], int *index, FILE *output) {
    (*index)-=2;
    fprintf(output, "BT@%d,%d<-%d,%d\n", backtracking[*index][0], backtracking[*index][1], coords[0], coords[1]);
    coords[0] = backtracking[*index][0];
    coords[1] = backtracking[*index][1];
}



int main(int argc, char* argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int l;
    fscanf(input, "%d", &l);
    
    for (int i=0; i<l; i++) {
        int colunas, linhas;
        fscanf(input, "%d %d", &colunas, &linhas);

        char **labirinto = (char **)malloc(linhas*sizeof(char*));
        int *inicio = (int *)malloc(2*sizeof(int));
        int *coords = (int *)malloc(2*sizeof(int));

        for (int j=0; j<linhas; j++) {
            labirinto[j] = (char *)malloc(colunas*sizeof(char));
            for (int k=0; k<colunas; k++) {
                fscanf(input, " %c", &labirinto[j][k]);
                if (labirinto[j][k] == 'X') {
                    inicio[0] = j;
                    inicio[1] = k;

                }
            }
        }

        int **backtracking = (int **)malloc(colunas*linhas*sizeof(int*));
        
        int index = 0; 
        int semSaida = 1;
        coords[0] = inicio[0];
        coords[1] = inicio[1];

        fprintf(output, "L%d:\n", i);
        fprintf(output, "INICIO@%d,%d\n", inicio[0], inicio[1]);
        
        while ((coords[0] != inicio[0] || coords[1] != inicio[1]) || !beco(labirinto, coords)) {
            
            labirinto[coords[0]][coords[1]] = '1';
            addBacktracking(backtracking, coords, &index);
            if (borda(coords, colunas, linhas)) {
                fprintf(output, "SAIDA@%d,%d\n", coords[0], coords[1]);
                semSaida = 0;                
                break;
            } else if (labirinto[coords[0]][coords[1]+1] == '0'){
                goRight(coords, output); //right
            } else if (labirinto[coords[0]-1][coords[1]] == '0'){
                goUp(coords, output); //Up
            } else if (labirinto[coords[0]][coords[1]-1] == '0'){
                goLeft(coords, output); //left
            } else if (labirinto[coords[0]+1][coords[1]] == '0'){
                goDown(coords, output); //down
            } else {
                backtrack(backtracking, coords, &index, output);
            }
            
        }

        if (semSaida) {
            fprintf(output, "SEM_SAIDA\n");
        }
    }
}
int KMP ( int * tabela , char * texto , char * padrao, int tam_sub) {
    
    int contador = 0;
    int n = strlen ( texto ) , m = strlen ( padrao ) ;
    calcular_tabela (tabela, padrao);

    for ( int i = 0 , j = -1; i < n ; i ++) {
        printf("padrao[%d]: %c\n", j + 1, padrao[j + 1] );
        printf("texto[%d]: %c\n", i, texto[i]);
        printf("\n");

        while ( j >= 0 && padrao[j + 1] != texto [i]) {
            if ((j + 1) >= tam_sub){
                contador += j + 1;

                int c = 0;
                char *aux = padrao;
                padrao = (char *) malloc(sizeof(char) * (m));
                for (int w = j+1; w < m; w++) {
                    padrao[c++] = aux[w];
                }
                padrao[c] = '\0';

                calcular_tabela (tabela, padrao);
                j =-1;
                
                if (strlen(padrao) < tam_sub) {
                    break;
                }

            } else {
                j = tabela [ j ];
            }

        }
        
        if( padrao[j + 1] == texto[i]) {j++;}

        printf("j: %d\n", j);

        if(j == m - 1) {
            
            if ( j + 1 >= tam_sub){
                contador += j + 1;
                

                return contador;
                
            } else {
                break;
            }
            
         }
        
    }
    return contador;
}
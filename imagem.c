/*+----------------------------------------------------------------------------------+
  | PROCESSAMENTO DE IMAGEM / AEDs 2                                                 |
  | -------------------------------------------------------------------------------- |
  |  Vitor Risso Parisi    RA: 2016.1.08.037                                         |
  |  OBS: Partes dos codigos aproveitas de quando realizei processamento de Imagem.  |                                                                    |
  |                                                                                  |
  +----------------------------------------------------------------------------------+*/
#include <stdio.h>
#include <math.h>

void binarizar(int *img1, int *img2, int nl, int nc, int fator) {
    for (int *p = img1, *q = img2; p < img1 + nl * nc; p++, q++){
        *q = *p > fator ? 255 : 0;
        //VALOR == *(p+1)== p[1]   ||Endereco p ==  p+1 == &p[1]
        //p+0   = posicao 0 >    [nl][nc] => [0][0]
        //p+1   = posicao 1 >    [nl][nc] => [0][1]
        //p+719 = posicao 719 >  [nl][nc] => [0][719] 
        //p+720 = posicao 720 >  [nl][nc] => [1][0]        
               
        //printf(" %d, %d , %d ,%d", p, *p, p+1, p[1] );
       
        
    }
       
}
void escurecerBordas(int *img1, int *img2, int nl, int nc) {
    int i, j;
    float v;
    for(int x=0, *p = img1, *q = img2; p< img1 + nl * nc;p++, x++){
        *(q+x) = *p; 
    }
    v = 0.3;
    int n = 0;
    for (n = 0; n < 30; n++) {
        for (int x = 0, *p = img1, *q = img2; p < img1 + nl * nc; p++, x++) {           
            if(x >= nc*n+n && x< nc*(n+1)-n ){
                *(q + x) = *p * v;
            }
            if(x > nc*(nl-1-n)+n &&   x < ((nl-n)*nc)-1-n) {
                *(q + x) = *p * v;
            }
            if( x%nc==n && x>=nc*n){
                *(q + x) = *p * v; 
            }
            if( x%nc==(nc-n-1) && x>=(nc-n-1)*n ){
                *(q + x) = *p * v; 
            }         
        }
        v = v + 0.03;
   }   
}

void eliminarRuidos(int *img1, int *img2, int nl, int nc ){
    int *p, *q, *x, *aux, m, alt = 1;
    for (p = img1 + nc + 1, x = img2 + nc + 1;p < img1 + (nc*nl) - nc - 1;p++, x++){
        aux = p+1;
        q = p - nc - 1;
        m += *q;
        q++;
        m += (*q) * (2);
        q++;
        m += *q;
        q += nc;
        m += (*q) * (2);
        q += nc;
        m += *q;
        q--;
        m += (*q) * (2);
        q--;
        m += *q;
        q -= nc;
        m += (*q) * (2);
        m += (*p) * (4);
        m = m/16;
        
        if (m > 255)
            m = 255;
        if (m < 0)
            m = 0;
        *x = m;
        if(aux == img1 + (nc*alt) + nc - 1){
            p+=2;
            x+=2;
            alt++;
        }
    }
        
}

void escurecerBordas0(int **img1, int **img2, int nl, int nc) {
    int i, j;
    float v;

    for (i = 0; i < nl; i++) {
      for (j = 0; j < nc; j++){
        *(*(img2) + i * nc + j )  = *(*(img1) + i * nc + j);
        }
    }
    
    v = 0.3;
    for(int n = 0 ; n<10 ; n++){
        for (i=0+n , j = 0+n; j <= nc-n; j++){
            *(*(img2) + i * nc + j) = *(*(img1) + i * nc + j) * v;
        }
        for(j=nc-n, i = 1+n;  i<=nl-n; i++){
            *(*(img2) + i * nc + j) = *(*(img1) + i * nc + j) * v;
        }
        for (j=0+n, i =1+n; i <= nl-n; i++){
            *(*(img2) + i * nc + j) = *(*(img1) + i * nc + j) * v;
        }
        for (i = nl-n, j = 1+n; j<= nc-1-n ; j++){
             *(*(img2) + i * nc + j) = *(*(img1) + i * nc + j) * v;
        }
        v = v + 0.03;
    }
    
}

void eliminarRuidos0(int **img1, int **img2, int nl, int nc) {
    int mask[3][3] = {1, 2, 1,
                      2, 4, 2, 
                      1, 2, 1};
    int i, j, x, y;    
    for (x = 1; x < nl-1; x++) {
        for (y = 1; y < nc-1; y++) {
                *(*(img2) + x * nc + y) = 
                   ((*(*(img1) + (x-1) * nc + y-1) * mask[0][0]) + 
                    (*(*(img1) + (x-1) * nc + y)   * mask[0][1]) + 
                    (*(*(img1) + (x-1) * nc + y+1) * mask[0][2]) +
                    
                    (*(*(img1) + x * nc + y-1)  * mask[1][0]) +
                    (*(*(img1) + x * nc + y)    * mask[1][1]) +
                    (*(*(img1) + x * nc + y+1)  * mask[1][2]) +
                     
                    (*(*(img1) + (x+1) * nc + y-1)* mask[2][0]) +
                    (*(*(img1) + (x+1) * nc + y)  * mask[2][1]) +
                    (*(*(img1) + (x+1) * nc + y+2)* mask[2][2])) / 16;
        }
    }
}

void gerarIcone0(int **img1, int **img2, int nl, int nc) {
    int i, j, a, b, inl, inc, x = 0, y = 0;
    inl = floor(nl / 64);
    inc = floor(nc / 64);
    int reso = inl * inc;
    int aux = 0;

    for (i = 0; i < 64; i++) {
        for (j = 0; j < 64; j++) {
            for (a = x; a < (inl + x); a++) {
                for (b = y; b < (inc + y); b++) {
                    if (a <= nc && b <= nc)
                        aux = aux + (*(*(img1) + a * nc + b));
                }
            }
            aux = aux / reso;
            *(*(img2) + i * 64 + j) = aux;
            aux = 0;
            y = y + inc;
        }
        y = 0;
        x = x + inl;
    }
}


/*+----------------------------------------------------------------------------------+
  | PROCESSAMENTO DE IMAGEM / AEDs 2                                                 |
  | -------------------------------------------------------------------------------- |
  |  Vitor Risso Parisi    RA: 2016.1.08.037                                         |
  |  OBS: Partes dos codigos aproveitas de quando realizei processamento de Imagem.  |                                                                    |
  |                                                                                  |
  +----------------------------------------------------------------------------------+*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include "imagem.h"

#define TRUE 1
#define FALSE 0
#define CREATOR "# CREATOR: Vitor Risso Parisi\t RA:2016.1.08.037\t Processamento Imagem\n"

typedef int *imagem;

int aloca_memo(imagem *Img, int nl, int nc) {
    *Img = (int *) malloc(nl * nc * sizeof (int));
    if (*Img)
        return TRUE;
    else
        return FALSE;
}

int desaloca_memo(imagem *Img) {
    free(*Img);
}

void info_imagem(char *nome, int nl, int nc, int mn) {
    printf("\nINFORMACOES SOBRE A IMAGEM:");
    printf("\n--------------------------\n");
    printf("Nome do arquivo.............: %s \n", nome);
    printf("Numero de linhas............: %d \n", nl);
    printf("Numero de colunas...........: %d \n", nc);
    printf("Maximo nivel-de-cinza/cores.: %d \n\n", mn);
}

int le_imagem_pgm(char *nome, imagem *Img, int *nl, int *nc, int *mn) {
    int i, j, k, LIMX, LIMY, MAX_NIVEL;
    char c, LINHA[100];
    FILE *arq;
    if ((arq = fopen(nome, "r")) == NULL) {
        printf("Erro na ABERTURA do arquivo <%s>\n\n", nome);
        return FALSE;
    }
    /*-- PGM = "P2" -----------*/
    fgets(LINHA, 80, arq);
    if (!strstr(LINHA, "P2")) {
        printf("Erro no FORMATO do arquivo <%s>\n\n", nome);
        return FALSE;
    }
    /*-- Dimensoes da imagem --*/
    fgets(LINHA, 80, arq);
    do {
        fgets(LINHA, 80, arq);
    } while (strchr(LINHA, '#'));
    sscanf(LINHA, "%d %d", &LIMX, &LIMY);
    fscanf(arq, "%d", &MAX_NIVEL);

    if (LIMX == 0 || LIMY == 0 || MAX_NIVEL == 0) {
        printf("Erro nas DIMENSOES do arquivo <%s>\n\n", nome);
        return FALSE;
    }

    if (aloca_memo(Img, LIMY, LIMX)) {
        for (i = 0; i < LIMY; i++) {
            for (j = 0; j < LIMX; j++) {
                fscanf(arq, "%d", &k);
                if (k > MAX_NIVEL) {
                    printf("Erro nos DADOS do arquivo <%s>\n", nome);
                    printf("Valor lido: %d   Max Nivel: %d\n\n", k, MAX_NIVEL);
                    return FALSE;
                }
                *(*(Img) + i * LIMX + j) = k;
            }
        }
        fclose(arq);
    } else {
        printf("Erro na ALOCACAO DE MEMORIA para o arquivo <%s>\n\n", nome);
        printf("Rotina: le_imagem_pgm\n\n");
        fclose(arq);
        return FALSE;
    }
    *nc = LIMX;
    *nl = LIMY;
    *mn = MAX_NIVEL;
    return TRUE;
}

void grava_imagem_pgm(imagem Img, char *nome, int nl, int nc, int mn) {
    int i, j, x, k, valores_por_linha;
    FILE *arq;
    if ((arq = fopen(nome, "wt")) == NULL) {
        printf("Erro na CRIACAO do arquivo <%s>\n\n", nome);
    } else {
        fputs("P2\n", arq);
        fputs(CREATOR, arq);
        fprintf(arq, "%d  %d\n", nc, nl);
        fprintf(arq, "%d\n", mn);
        valores_por_linha = 16;
        for (i = 0, k = 0; i < nl; i++)
            for (j = 0; j < nc; j++) {
                x = *(Img + i * nc + j);
                fprintf(arq, "%3d ", x);
                k++;
                if (k > valores_por_linha) {
                    fprintf(arq, "\n");
                    k = 0;
                }
            }
    }
    fclose(arq);
}

void erro(char *msg) {
    printf("\n\nERRO: %s\n", msg);
    exit(10);
}

void msg(char *s) {
    printf("\nFast FOURIER de uma imagem qualquer");
    printf("\n-------------------------------");
    printf("\nUSO.:  %s  <IMG.PGM>", s);
    printf("\nONDE:\n");
    printf("    <IMG.PGM>  Arquivo da imagem em formato PGM\n\n");
}

/*------------------------------------------------------
 *        P R O G R A M A    P R I N C I P A L
 *------------------------------------------------------*/
int main(int argc, char *argv[]) {
    int OK, nc, nl, mn;
    char nome[100];
    char comando[100];
    imagem In, Out;
    if (argc < 2)
        msg(argv[0]);
    else {
        OK = le_imagem_pgm(argv[1], &In, &nl, &nc, &mn);
        if (OK) {
            printf("\nProcessar Imagens - AEDs 2 - Trabalho 1");
            printf("\nVitor Risso Parisi.........: 2016.1.08.037\n");
            info_imagem(argv[1], nl, nc, mn);
            aloca_memo(&Out, nl, nc);
            
          
            binarizar(In, Out, nl, nc, 96);
            strcpy(nome, argv[1]);
            strcat(nome, "Binaria.pgm");
            grava_imagem_pgm(Out, nome, nl, nc, mn);
            
            escurecerBordas(In, Out, nl, nc);
            strcpy(nome, argv[1]);
            strcat(nome, "Bordas.pgm");
            grava_imagem_pgm(Out, nome, nl, nc, mn);
            
            eliminarRuidos(In, Out, nl, nc );
            strcpy(nome, argv[1]);
            strcat(nome, "Ruidos.pgm");
            grava_imagem_pgm(Out, nome, nl, nc, mn);
            
            //escurecerBordas0(&In, &Out, nl, nc);
            //eliminarRuidos0(&In, &Out, nl, nc);
            
            gerarIcone0( &In, &Out, nl, nc);
            nl=64;
            nc=64;
            strcpy(nome, argv[1]);
            strcat(nome, "Icone.pgm");
            grava_imagem_pgm(Out, nome, nl, nc, mn);
            
            desaloca_memo(&In);
            desaloca_memo(&Out);
        }
    }
    return 0;
}

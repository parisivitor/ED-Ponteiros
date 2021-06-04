/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   imagem.h
 * Author: vitim
 *
 * Created on 17 de Dezembro de 2020, 17:45
 */

#ifndef IMAGEM_H
#define IMAGEM_H

#ifdef __cplusplus
extern "C" {
#endif

void binarizar(int *img1, int *img2, int nl, int nc, int fator);
void escurecerBordas0(int **img1, int **img2, int nl, int nc);
void eliminarRuidos0(int **img1, int **img2, int nl, int nc);
void gerarIcone0(int **img1, int **img2, int nl, int nc);

void escurecerBordas(int *img1, int *img2, int nl, int nc);
void eliminarRuidos(int *img1, int *img2, int nl, int nc );



#ifdef __cplusplus
}
#endif

#endif /* IMAGEM_H */


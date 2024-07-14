#include "definitions.h"
#include <iostream>
#include <cmath>
#include <stdio.h>

using namespace std;

Cor cor;
Coordenada inicial, final;

int xy_to_array(unsigned int x, unsigned int y)   //converte coordenadas (x,y) do pixel para seu indice no array do Frame Buffer
{
    //mantem as coordenadas dentro da resolução da tela
    if(x >= IMAGE_WIDTH) x = IMAGE_WIDTH-1;
    if(y >= IMAGE_HEIGHT) y = IMAGE_HEIGHT-1;

    return y*4*IMAGE_WIDTH + x*4;
}

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
    //*************************************************************************
    // Chame aqui as funções do mygl.h
    //*************************************************************************
    cor.R = cor.G = cor.B = cor.A = 255;
    inicial.x = 64;
    inicial.y = 86;
    PutPixel(cor, inicial);

    inicial.x = 256;
    inicial.y = 256;

    cor.G = cor.R = 127;
    final.x = 300;
    final.y = 0;
    //DrawLine(cor, inicial, final);
    
    //testa todas as linhas da metade direita da tela
    for (; final.x < 512; final.x++) {
        DrawLine(cor, inicial, final);
    }
    for (; final.y < 512; final.y++) {
        DrawLine(cor, inicial, final);
    }
    for (; final.x > 256; final.x--) {
        DrawLine(cor, inicial, final);
    }
}

void PutPixel(Cor cor, Coordenada posicao) {
    int indice = xy_to_array(posicao.x, posicao.y);

    // Escreve um pixel na tela da cor e posição especificada
    FBptr[indice+0] = cor.R; // componente R
    FBptr[indice+1] = cor.G; // componente G
    FBptr[indice+2] = cor.B; // componente B
    FBptr[indice+3] = cor.A; // componente A
}

void DrawLine(Cor cor, Coordenada i, Coordenada f)
{
    int dy = f.y - i.y;
    int dx = f.x - i.x;
    Coordenada atual = i; 
    int e = 0;
    int m = 0;

    //lógica para determinar o octante a ser rasterizado
    //considerar os octantes espelhados no eixo y
    if (MODULO(dx)>MODULO(dy) && dx>0 && dy>=0)     //primeiro octante inferior direito
        m = 0;
    else if (MODULO(dx)>MODULO(dy) && dx>0 && dy<0) //primeiro octante superior direito
        m = 1;
    else if (MODULO(dx)<MODULO(dy) && dx>=0 && dy>0)//segundo octante inferior direito
        m = 2;
    else if (MODULO(dx)<MODULO(dy) && dx>=0 && dy<0)//segundo octante superior direito
        m = 3;
    
    switch (m)
    {
    case 0:
        while (atual.x <= f.x)
        {
            PutPixel(cor, atual);
            atual.x++;
            e += 2*dy;

            if(e >= dx){
                atual.y++;
                e -= 2*dx;
            }
        }
        break;
    
    case 1:
        while (atual.x <= f.x)
        {
            PutPixel(cor, atual);
            atual.x++;
            e += -2*dy; 

            if(e >= dx){
                atual.y--;
                e -= 2*dx;
            }
        } 
        break;

    case 2:
        while (atual.y <= f.y)  //para um dy>dx, inverte a lógica de parada para o eixo y
        {
            PutPixel(cor, atual);
            atual.y++;
            e += 2*dx;

            if(e >= dy){
                atual.x++;
                e -= 2*dy;
            }
        }
        break;

    case 3: //ERRO?: chega na coordenada (512,0) -> esperado (511,0)
        while (atual.y >= f.y)
        {
            PutPixel(cor, atual);
            printf("(%d,%d)\n",atual.x,atual.y);    //percorre as coordenadas mas não liga os pixels
            atual.y--;
            e += 2*dy;

            if(e <= dy){
                atual.x++;
                e -= -2*dx;
            }
        }
        break;

    default:
        cout << "Nenhum caso\n";
        break;
    }
}
/*
void DrawTriangle(void)
{

}
*/
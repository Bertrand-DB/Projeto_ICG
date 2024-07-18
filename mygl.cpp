#include "definitions.h"
#include <iostream>
#include <cmath>
#include <stdio.h>

using namespace std;


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
    Cor cor;
    Coordenada inicial, final, media;
    
    cor = {255, 255, 255, 255};
    inicial.x = 64;
    inicial.y = 86;
    PutPixel(cor, inicial);
    
    //Teste do DrawLine
    inicial = {IMAGE_HEIGHT/2, IMAGE_WIDTH/2};

    final = {256, 0};
    cor = {255, 0, 0, 255};//Vermelho
    for(; final.x <= 512; final.x++) {
        DrawLine(cor, inicial, final);
    }
    
    final = {512, 0};
    cor = {0, 255, 0, 255};//Verde
    for(; final.y <= 256; final.y++) {
        DrawLine(cor, inicial, final);
    }
    
    final = {512, 256};
    cor = {0, 0, 255, 255};//Azul
    for(; final.y <= 512; final.y++){
        DrawLine(cor, inicial, final);
    }

    final = {256, 512};
    cor = {128, 0, 128, 255};//Roxo
    for(; final.x <= 512; final.x++){
        DrawLine(cor, inicial, final);
    }

    final = {0,0};
    cor = {255, 255, 0, 255};//Amarelo
    for(; final.x <= 256; final.x++){
        DrawLine(cor, inicial, final);
    }
    
    final = {0,0};
    cor = {0, 255, 255, 255};//Ciano
    for(; final.y <= 256; final.y++){
        DrawLine(cor, inicial, final);
    }
    
    final = {0,256};
    cor = {255, 0, 255, 255};//Magenta
    for(; final.y <= 512; final.y++){
        DrawLine(cor, inicial, final);
    }
    
    final = {0,512};
    cor = {255, 165, 0, 255};//Laranja
    for(; final.x <= 256; final.x++){
        DrawLine(cor, inicial, final);
    }

    inicial = {135,120};
    media = {326,199};
    final = {236,343};
    cor = {0,0,0, 255};
    
    DrawTriangle(cor, inicial, media, final);
    
}

void PutPixel(Cor cor, Coordenada posicao) {
    int indice = xy_to_array(posicao.x, posicao.y);

    // Escreve um pixel na tela da cor e posição especificada
    FBptr[indice+0] = cor.R; // componente R
    FBptr[indice+1] = cor.G; // componente G
    FBptr[indice+2] = cor.B; // componente B
    FBptr[indice+3] = cor.A; // componente A
}

void DrawLine(Cor cor, Coordenada i, Coordenada f) {
    // Calcula as diferenças absolutas entre as coordenadas finais e iniciais
    int dx = abs(f.x - i.x);
    int dy = abs(f.y - i.y);
    
    // Determina a direção do incremento (positivo ou negativo) para x e y
    int sx = i.x < f.x ? 1 : -1;  // Se i.x < f.x, incrementa x, caso contrário, decrementa
    int sy = i.y < f.y ? 1 : -1;  // Se i.y < f.y, incrementa y, caso contrário, decrementa
    
    // Inicializa o erro para balancear o incremento de x e y
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    // Define a coordenada atual como a inicial
    Coordenada atual = i;

    // Loop principal que desenha a linha
    while (true) {
        // Desenha o pixel na coordenada atual com a cor especificada
        PutPixel(cor, atual);
        
        // Verifica se a coordenada atual atingiu a coordenada final
        if (atual.x == f.x && atual.y == f.y) break;
        
        // Armazena o valor atual do erro em e2
        e2 = err;
        
        // Ajusta as coordenadas e o erro com base nos valores de dx e dy
        if (e2 > -dx) {  // Se e2 for maior que -dx, ajusta x
            err -= dy;
            atual.x += sx;
        }
        if (e2 < dy) {  // Se e2 for menor que dy, ajusta y
            err += dx;
            atual.y += sy;
        }
    }
}

void DrawTriangle(Cor cor, Coordenada a, Coordenada b, Coordenada c)
{
    DrawLine(cor, a, b);
    DrawLine(cor, a, c);
    DrawLine(cor, b, c);
}

#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#define IMAGE_WIDTH  512 // número de colunas da imagem.
#define IMAGE_HEIGHT 512 // número de linhas da imagem.
#define MODULO(x) ((x)>=0?(x):-(x))

unsigned char * FBptr;

struct Cor
{
    unsigned int R, G, B, A;
};

struct Coordenada
{
    int x, y;
};

#endif // _DEFINITIONS_H_

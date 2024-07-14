#ifndef _MYGL_H_
#define _MYGL_H_
#endif

#include "definitions.h"

//-----------------------------------------------------------------------------
void MyGlDraw(void);

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************

void PutPixel(Cor cor, Coordenada posicao);
void DrawLine(Cor cor, Coordenada inicial, Coordenada final);
void DrawTriangle(void);
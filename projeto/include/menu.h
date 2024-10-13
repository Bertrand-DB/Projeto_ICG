#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <GL/glut.h>

// Dados do menu e do pop-up
extern const char* menuItems[];
extern const char* popItems[];

extern int selectedItem;
extern bool showControlersInfo;

// Função para renderizar texto bitmap
void renderBitmapString(float x, float y, void *font, const char *string);

// Função de renderização do menu
void renderMenu();

#endif // MENU_H
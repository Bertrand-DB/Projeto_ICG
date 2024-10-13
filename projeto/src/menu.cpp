#include "../include/menu.h"

// Dados do menu e do pop-up
const char* menuItems[] = { "Sair do programa", "Controles" };

int selectedItem = -1;
bool showControlersInfo = false;

// Função para renderizar texto bitmap
void renderBitmapString(float x, float y, void *font, const char *string) {
    const char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// Função de renderização do menu
void renderMenu() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    gluOrtho2D(0, 1660, 0, 960); // Configuração do sistema de coordenadas 2D

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < 2; i++) {
        if (i == selectedItem) {
            glColor3f(0.0, 0.0, 0.0); // Cor do texto do item selecionado
        } else {
            glColor3f(1.0, 1.0, 1.0); // Cor do texto dos itens não selecionados
        }

        renderBitmapString(10, 30 + (i * 20), GLUT_BITMAP_9_BY_15, menuItems[i]);

        if (i == selectedItem) {
            glColor3f(1.0, 1.0, 1.0); // Fundo branco para o item selecionado
            glRectf(8, 22 + (i * 20), 200, 45 + (i * 20)); // Desenha o retângulo atrás do texto
        } else {
            glColor4f(0.2, 0.2, 0.8, 0.4); // Fundo com transparência para itens não selecionados
            glRectf(8, 18 + (i * 20), 200, 40 + (i * 20)); // Desenha o retângulo atrás do texto
        }
    }

    glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glFlush();
}
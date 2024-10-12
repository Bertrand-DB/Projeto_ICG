#include "../include/menu.h"

// Dados do menu e do pop-up
const char* menuItems[] = { "Sair do programa", "Controles" };
const char* popItems[] = {
    "'ESC' - Tecla ESC para voltar",
    " ",
    "'l' - Desenhar as orbitas dos planetas",
    "'r' - Reposiciona a camera na posição inicial",
    " ",
    "'3' - Define a velocidade das orbitas",
    "'2' - Define a velocidade das orbitas",
    "'1' - Define a velocidade das orbitas",
    " ",
    "'p' - Pausar/retomar o movimento com a tecla P",
    " ",
    "'j' - Rotacionar para baixo (verticalmente)",
    "'u' - Rotacionar para cima (verticalmente)",
    "'k' - Rotacionar para a direita (horizontalmente)",
    "'h' - Rotacionar para a esquerda (horizontalmente)",
    " ",
    "'c' - Mover a camera para baixo (eixo y)",
    "'Espaco' - Mover a camera para cima (eixo y)",
    "'s' - Mover a camera para tras",
    "'w' - Mover a camera para frente",
    "'d' - Mover a camera para a direita",
    "'a' - Mover a camera para a esquerda"
};

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

// Função para renderizar o pop-up com controles
void renderPopUp() {
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

    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    int popupWidth = windowWidth / 2;
    int popupHeight = windowHeight / 2;
    int popupX = (windowWidth - popupWidth) / 2;
    int popupY = (windowHeight - popupHeight) / 2;

    glColor3f(1.0, 1.0, 1.0); // Cor do texto branco
    for (int i = 0; i < 22; i++) {
        renderBitmapString(popupX + 10, popupY + 50 + (i * 15), GLUT_BITMAP_9_BY_15, popItems[i]);
    }

    renderBitmapString(popupX + 200, popupY + 400, GLUT_BITMAP_TIMES_ROMAN_24, "Controles do Programa");

    glColor4f(0.0, 0.0, 0.0, 0.8); // Fundo preto com transparência
    glRectf(popupX, popupY, (popupX + popupWidth) - 200, (popupY + popupHeight)); // Desenha o pop-up

    glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glFlush();
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
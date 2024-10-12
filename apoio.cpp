#include <GL/glut.h>
#include <string.h>
#include <iostream>

#define WINDOW_W 800
#define WINDOW_H 800

// Dados do menu
const char* menuItems[] = { "Sair do programa", "Controles"};
const char* popItems[] = {
    "'a' - Mover a camera para a esquerda",
    "'d' - Mover a camera para a direita",
    "'w' - Mover a camera para frente",         
    "'s' - Mover a camera para tras",           
    "'Espaco' - Mover a camera para cima (eixo y)",            
    "'c' - Mover a camera para baixo (eixo y)",            
    "'h' - Rotacionar para a esquerda (horizontalmente)",            
    "'k' - Rotacionar para a direita (horizontalmente)",          
    "'u' - Rotacionar para cima (verticalmente)",           
    "'j' - Rotacionar para baixo (verticalmente)",
    "'p' - Pausar/retomar o movimento com a tecla P",            
    "'1' - Define a velocidade das orbitas",           
    "'2' - Define a velocidade das orbitas",            
    "'3' - Define a velocidade das orbitas",            
    "'r' - Reposiciona a camera na posição inicial",
    "'l' - Desenhar as orbitas dos planetas",
    "'ESC' - Tecla ESC para voltar"};
int selectedItem = 0;
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
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    int popupWidth = windowWidth / 3;
    int popupHeight = windowHeight / 3;
    int popupX = (windowWidth - popupWidth) / 2;
    int popupY = (windowHeight - popupHeight) / 2;

    glColor4f(0.0, 0.0, 0.0, 0.8); // Fundo preto com transparência
    glRectf(popupX, popupY, popupX + popupWidth, popupY + popupHeight); // Desenha o pop-up

    glColor3f(1.0, 1.0, 1.0); // Cor do texto branco

    for (int i = 0; i < 17; i++) {
        renderBitmapString(popupX + 10, popupY + 50 + (i * 15), GLUT_BITMAP_9_BY_15, popItems[i]);
    }

    renderBitmapString(popupX + 200, popupY + 305, GLUT_BITMAP_TIMES_ROMAN_24, "Controles do Programa");
}
    
// Função de renderização do menu
void renderMenu() {
    for (int i = 0; i < 2; i++) {
        if (i == selectedItem) {
            glColor3f(1.0, 1.0, 1.0); // Fundo branco para o item selecionado
            glRectf(8, 18 + (i * 20), 120, 38 + (i * 20)); // Desenha um retângulo atrás do texto
            glColor3f(0.0, 0.0, 0.0); // Cor do texto preto
        } else {
            glColor4f(0.2, 0.2, 0.8, 0.4); // Fundo cinza para itens não selecionados
            glRectf(8, 18 + (i * 20), 120, 38 + (i * 20)); // Desenha um retângulo atrás do texto
            glColor3f(1.0, 1.0, 1.0); // Cor do texto branco
        }
        renderBitmapString(10, 20 + (i * 20), GLUT_BITMAP_9_BY_15, menuItems[i]);
    }
}

// Função de display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (showControlersInfo) {
        renderPopUp();
    } else {
        renderMenu();
    }

    glFlush();//Não tem no codigo 
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            std::cout << "Seta para cima pressionada" << std::endl;
            break;
        case GLUT_KEY_DOWN:
            std::cout << "Seta para baixo pressionada" << std::endl;
            break;
        case GLUT_KEY_LEFT:
            std::cout << "Seta para esquerda pressionada" << std::endl;
            break;
        case GLUT_KEY_RIGHT:
            std::cout << "Seta para direita pressionada" << std::endl;
            break;
    }
}



// Função de controle do teclado
void keyboard(unsigned char key, int x, int y) {
    if (showControlersInfo) {
        if (key == 27) { // Esc para fechar o pop-up
            showControlersInfo = false;
        }
    } else {
        switch (key) {
            case GLUT_KEY_UP: // Sobe no menu
                selectedItem++;
                if (selectedItem > 1) selectedItem = 1;
                break;
            case GLUT_KEY_DOWN: // Desce no menu
                selectedItem--;
                if (selectedItem < 0) selectedItem = 0;
                break;
            case 13: // Enter
                if (selectedItem == 0) {
                    exit(0);
                } else if (selectedItem == 1) {
                    showControlersInfo = true;
                }
                break;
        }
    }
    glutPostRedisplay();
}

// Função de reshape para ajustar as coordenadas ao redimensionar a janela
void reshape(int w, int h) {
    glViewport(0, 0, w, h); // Ajusta o viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, w, 0.0, h); // Define o sistema de coordenadas de acordo com o novo tamanho
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Função de inicialização
void init() {
    glEnable(GL_BLEND); // Habilita o blending (mistura de cores)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Define a função de blend
    glClearColor(1.0, 1.0, 0.0, 0.0); // Cor de fundo amarela
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutCreateWindow("Menu Personalizado");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape); // Adiciona a função de reshape para redimensionamento
    glutMainLoop();
    return 0;
}

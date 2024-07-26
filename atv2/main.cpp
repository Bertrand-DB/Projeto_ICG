#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

GLint WINDOW_WIDTH = 500, WINDOW_HEIGHT = 500;
GLfloat corAtual[] = {0.0f, 0.0f, 0.0f}; // Cor atual para pontos e linhas

enum ModoDesenho { PONTO, LINHA, RETANGULO, CIRCULO };
ModoDesenho modoAtual = PONTO; // Modo de desenho inicial

struct Ponto {
    GLfloat x, y; // Coordenadas do ponto
    GLfloat r, g, b; // Cor do ponto
};

// Lista de pontos, linhas e retângulos desenhados
std::vector<Ponto> pontos;
std::vector<std::pair<Ponto, Ponto>> linhas;
std::vector<std::pair<Ponto, Ponto>> retangulos;
std::vector<std::pair<Ponto, GLfloat>> circulos;
std::vector<Ponto> pontosLinha; // Para armazenar os pontos clicados para desenhar linhas e retângulos

bool mousePressed = false; // Estado do botão do mouse

// Declaração da função menu antes de ser usada
void menu(int option);

void initializeGlut(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Desenho com OpenGL");
}

void initialize() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha todas as linhas
    for (const auto& linha : linhas) {
        glColor3f(linha.first.r, linha.first.g, linha.first.b);
        glBegin(GL_LINES);
        glVertex2f(linha.first.x, linha.first.y);
        glVertex2f(linha.second.x, linha.second.y);
        glEnd();
    }

    // Desenha todos os retângulos
    for (const auto& retangulo : retangulos) {
        glColor3f(retangulo.first.r, retangulo.first.g, retangulo.first.b);
        glBegin(GL_LINE_LOOP);
        glVertex2f(retangulo.first.x, retangulo.first.y);
        glVertex2f(retangulo.first.x, retangulo.second.y);
        glVertex2f(retangulo.second.x, retangulo.second.y);
        glVertex2f(retangulo.second.x, retangulo.first.y);
        glEnd();
    }

    // Desenha todos os círculos
    for (const auto& circulo : circulos) {
        glColor3f(circulo.first.r, circulo.first.g, circulo.first.b);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 100; ++i) {
            GLfloat angle = 2.0 * M_PI * i / 100;
            GLfloat dx = circulo.second * cosf(angle);
            GLfloat dy = circulo.second * sinf(angle);
            glVertex2f(circulo.first.x + dx, circulo.first.y + dy);
        }
        glEnd();
    }

    // Desenha todos os pontos com suas cores associadas
    for (const auto& ponto : pontos) {
        glColor3f(ponto.r, ponto.g, ponto.b);
        glBegin(GL_POINTS);
        glVertex2f(ponto.x, ponto.y);
        glEnd();
    }

    glFlush();
}

void desenharPonto(GLfloat x, GLfloat y) {
    pontos.push_back(Ponto{(GLfloat)x, (GLfloat)y, corAtual[0], corAtual[1], corAtual[2]});
    glutPostRedisplay();
}

void desenharLinha() {
    if (pontosLinha.size() == 2) {
        linhas.push_back({pontosLinha[0], pontosLinha[1]});
        pontosLinha.clear(); // Limpa a lista de pontos para a próxima linha
        glutPostRedisplay();
    }
}

void desenharRetangulo() {
    if (pontosLinha.size() == 2) {
        retangulos.push_back({pontosLinha[0], pontosLinha[1]});
        pontosLinha.clear(); // Limpa a lista de pontos para o próximo retângulo
        glutPostRedisplay();
    }
}

void desenharCirculo() {
    if (pontosLinha.size() == 2) {
        GLfloat dx = pontosLinha[1].x - pontosLinha[0].x;
        GLfloat dy = pontosLinha[1].y - pontosLinha[0].y;
        GLfloat raio = sqrt(dx * dx + dy * dy);
        circulos.push_back({pontosLinha[0], raio});
        pontosLinha.clear(); // Limpa a lista de pontos para o próximo círculo
        glutPostRedisplay();
    }
}

void limparTela() {
    pontos.clear();
    linhas.clear();
    retangulos.clear();
    circulos.clear();
    pontosLinha.clear();
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressed = true;
            if (modoAtual == PONTO) {
                desenharPonto(x, y);
            } else if (modoAtual == LINHA) {
                if (pontosLinha.size() == 1) {
                    // Adiciona o ponto e desenha a linha
                    pontosLinha.push_back(Ponto{(GLfloat)x, (GLfloat)y, corAtual[0], corAtual[1], corAtual[2]});
                    desenharLinha();
                } else {
                    // Adiciona o primeiro ponto da linha
                    pontosLinha.push_back(Ponto{(GLfloat)x, (GLfloat)y, corAtual[0], corAtual[1], corAtual[2]});
                }
            } else if (modoAtual == RETANGULO) {
                if (pontosLinha.size() == 1) {
                    // Adiciona o ponto e espera o segundo ponto para desenhar o retângulo
                    pontosLinha.push_back(Ponto{(GLfloat)x, (GLfloat)y, corAtual[0], corAtual[1], corAtual[2]});
                    desenharRetangulo();
                } else {
                    // Adiciona o primeiro ponto do retângulo
                    pontosLinha.push_back(Ponto{(GLfloat)x, (GLfloat)y, corAtual[0], corAtual[1], corAtual[2]});
                }
            } else if (modoAtual == CIRCULO) {
                if (pontosLinha.size() == 1) {
                    // Adiciona o ponto e espera o segundo ponto para desenhar o círculo
                    pontosLinha.push_back(Ponto{(GLfloat)x, (GLfloat)y, corAtual[0], corAtual[1], corAtual[2]});
                    desenharCirculo();
                } else {
                    // Adiciona o primeiro ponto do círculo
                    pontosLinha.push_back(Ponto{(GLfloat)x, (GLfloat)y, corAtual[0], corAtual[1], corAtual[2]});
                }
            }
        } else if (state == GLUT_UP) {
            mousePressed = false;
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // Exibe o menu de opções
        glutCreateMenu(menu);
        glutAddMenuEntry("Ponto", PONTO);
        glutAddMenuEntry("Linha", LINHA);
        glutAddMenuEntry("Retangulo", RETANGULO);
        glutAddMenuEntry("Circulo", CIRCULO);
        glutAddMenuEntry("Limpar", 5); // Adiciona a opção de limpar ao menu
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }
}

void motion(int x, int y) {
    if (mousePressed && modoAtual == PONTO) {
        desenharPonto(x, y);
    }
}

void menu(int option) {
    if (option == 5) {
        limparTela();
    } else {
        modoAtual = static_cast<ModoDesenho>(option);
        glutPostRedisplay();
    }
}

void teclado(unsigned char key, int x, int y) {

    if(isupper(key)) key = tolower(key);//Transformar qq caracter em minusculo!

    switch (key) {
        case 'r':
            if (corAtual[0] == 1.0f && corAtual[1] == 0.0f && corAtual[2] == 0.0f) {
                // Se a cor atual é vermelho, redefine para preto
                corAtual[0] = 0.0f; corAtual[1] = 0.0f; corAtual[2] = 0.0f;
            } else {
                // Caso contrário, define a cor atual para vermelho
                corAtual[0] = 1.0f; corAtual[1] = 0.0f; corAtual[2] = 0.0f;
            }
            break;
        case 'g':
            if (corAtual[0] == 0.0f && corAtual[1] == 1.0f && corAtual[2] == 0.0f) {
                // Se a cor atual é verde, redefine para preto
                corAtual[0] = 0.0f; corAtual[1] = 0.0f; corAtual[2] = 0.0f;
            } else {
                // Caso contrário, define a cor atual para verde
                corAtual[0] = 0.0f; corAtual[1] = 1.0f; corAtual[2] = 0.0f;
            }
            break;
        case 'b':
            if (corAtual[0] == 0.0f && corAtual[1] == 0.0f && corAtual[2] == 1.0f) {
                // Se a cor atual é azul, redefine para preto
                corAtual[0] = 0.0f; corAtual[1] = 0.0f; corAtual[2] = 0.0f;
            } else {
                // Caso contrário, define a cor atual para azul
                corAtual[0] = 0.0f; corAtual[1] = 0.0f; corAtual[2] = 1.0f;
            }
            break;
    }
    // Redesenha a tela
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    initializeGlut(argc, argv);
    initialize();

    glutDisplayFunc(draw);
    glutMouseFunc(mouse);
    glutMotionFunc(motion); // Adiciona a função de movimentação do mouse
    glutKeyboardFunc(teclado);

    // Cria o menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Ponto", PONTO);
    glutAddMenuEntry("Linha", LINHA);
    glutAddMenuEntry("Retangulo", RETANGULO);
    glutAddMenuEntry("Circulo", CIRCULO);
    glutAddMenuEntry("Limpar", 5); // Adiciona a opção de limpar ao menu

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();

    return EXIT_SUCCESS;
}

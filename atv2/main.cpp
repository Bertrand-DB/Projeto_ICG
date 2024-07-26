#include <GL/glut.h>
#include <vector>

GLint WINDOW_WIDTH = 500, WINDOW_HEIGHT = 500;
GLfloat corAtual[] = {0.0f, 0.0f, 0.0f}; // Cor atual para pontos e linhas

enum ModoDesenho { PONTO, LINHA, RETANGULO };
ModoDesenho modoAtual = PONTO; // Modo de desenho inicial

struct Ponto {
    GLfloat x, y; // Coordenadas do ponto
    GLfloat r, g, b; // Cor do ponto
};

// Lista de pontos, linhas e retângulos desenhados
std::vector<Ponto> pontos;
std::vector<std::pair<Ponto, Ponto>> linhas;
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
    for (size_t i = 0; i < pontosLinha.size(); i += 2) {
        if (i + 1 < pontosLinha.size()) {
            Ponto p1 = pontosLinha[i];
            Ponto p2 = pontosLinha[i + 1];
            
            // Calcular os vértices do retângulo
            GLfloat x1 = p1.x;
            GLfloat y1 = p1.y;
            GLfloat x2 = p2.x;
            GLfloat y2 = p2.y;

            glColor3f(p1.r, p1.g, p1.b);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x1, y1);
            glVertex2f(x1, y2);
            glVertex2f(x2, y2);
            glVertex2f(x2, y1);
            glEnd();
        }
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
        Ponto p1 = pontosLinha[0];
        Ponto p2 = pontosLinha[1];
        
        // Adiciona os dois pontos da diagonal como retângulo
        linhas.push_back({p1, p2});
        pontosLinha.clear(); // Limpa a lista de pontos para o próximo retângulo
        glutPostRedisplay();
    }
}

void limparTela() {
    pontos.clear();
    linhas.clear();
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
                } else {
                    // Adiciona o segundo ponto e desenha o retângulo
                    pontosLinha.push_back(Ponto{(GLfloat)x, (GLfloat)y, corAtual[0], corAtual[1], corAtual[2]});
                    desenharRetangulo();
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
        glutAddMenuEntry("Limpar", 3); // Adiciona a opção de limpar ao menu
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }
}

void motion(int x, int y) {
    if (mousePressed && modoAtual == PONTO) {
        desenharPonto(x, y);
    }
}

void menu(int option) {
    if (option == 3) {
        limparTela();
    } else {
        modoAtual = static_cast<ModoDesenho>(option);
        glutPostRedisplay();
    }
}

void teclado(unsigned char key, int x, int y) {
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
    glutAddMenuEntry("Limpar", 3); // Adiciona a opção de limpar ao menu

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();

    return EXIT_SUCCESS;
}

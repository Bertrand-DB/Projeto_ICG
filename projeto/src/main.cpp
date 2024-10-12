//Comando de compilação: g++ main.cpp astro.cpp -o sis -lGL -lGLU -lSOIL  -lGLEW -lglut -lm

#include <cmath>
#include <cstdio>
#include <iostream>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include "../include/astro.h"

#define DISTANCIA_PADRAO    3.0f
#define COMPENSACAO         20.0f
#define RAIO_PADRAO         0.1f

// Ângulos de inclinação do eixo de rotação
#define INCLINACAO_EIXO_MERCURIO    0.01f
#define INCLINACAO_EIXO_VENUS       177.3f
#define INCLINACAO_EIXO_TERRA       23.26f
#define INCLINACAO_EIXO_MARTE       25.19f
#define INCLINACAO_EIXO_JUPITER     3.13f
#define INCLINACAO_EIXO_SATURNO     26.73f
#define INCLINACAO_EIXO_URANO       97.77f
#define INCLINACAO_EIXO_NETUNO      28.32f

// Astro(distancia, raio, inclinacaoEixo, anguloRotacao, anguloTranslacao, textura)
Astro sun(0.0f, 109*RAIO_PADRAO, 0.0f, 0.00f, 0.00f, 0);
Astro mercury(0.39 * DISTANCIA_PADRAO + COMPENSACAO, 0.38 * RAIO_PADRAO, INCLINACAO_EIXO_MERCURIO, 0.0f, 0.0f, 0);
Astro venus(0.72 * DISTANCIA_PADRAO + COMPENSACAO, 0.95 * RAIO_PADRAO, INCLINACAO_EIXO_VENUS, 0.0f, 0.0f, 0);
Astro earth(DISTANCIA_PADRAO + COMPENSACAO, RAIO_PADRAO, INCLINACAO_EIXO_TERRA, 0.0f, 0.0f, 0);
Astro mars(1.52*DISTANCIA_PADRAO+109*RAIO_PADRAO, 0.53*RAIO_PADRAO, INCLINACAO_EIXO_MARTE, 0.0f, 0.0f, 0);
Astro jupiter(5.2*DISTANCIA_PADRAO+109*RAIO_PADRAO, 11.21*RAIO_PADRAO, INCLINACAO_EIXO_JUPITER, 0.0f, 0.0f, 0);
Astro saturn(9.58*DISTANCIA_PADRAO+109*RAIO_PADRAO, 9.45*RAIO_PADRAO, INCLINACAO_EIXO_SATURNO, 0.0f, 0.0f, 0);
Astro uranus(19.18*DISTANCIA_PADRAO+109*RAIO_PADRAO, 4.01*RAIO_PADRAO, INCLINACAO_EIXO_URANO, 0.0f, 0.0f, 0);
Astro neptune(0.07*DISTANCIA_PADRAO+109*RAIO_PADRAO, 3.88*RAIO_PADRAO, INCLINACAO_EIXO_NETUNO, 0.0f, 0.0f, 0);

GLuint backgroundTexture;

float velOrbitalPadrao = 0.5f;
float velRotacaoPadrao = 1.0f;

// Posição espacial da câmera
float cameraX = 2.97471;
float cameraY = 46.4001;
float cameraZ = 38.8908;

// Posição angular da câmera
float cameraAngleH = 0.58;
float cameraAngleV = -0.8;

float movementSpeed = 0.2f;  // Velocidade de movimento da câmera
float rotationSpeed = 0.02f;  // Velocidade de rotação da câmera

// Vetor direção da câmera
float cameraLookX = 0.0f, cameraLookY = 0.0f, cameraLookZ = -1.0f;

// Variável para pausar o movimento de translação
bool translacao = true;
bool rotacao = true;
bool desenhaOrbita = true;

// Variáveis para controlar as teclas de movimento
bool moveUp = false;
bool moveDown = false;
bool moveLeft = false;
bool moveRight = false;
bool moveFoward = false;
bool moveBackward = false;
bool lookUp = false;
bool lookDown = false;
bool lookLeft = false;
bool lookRight = false;

GLuint loadTexture(const char* path) {
    GLuint texture = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!texture) {
        std::cerr << "Falha ao carregar a textura: " << path << std::endl;
        exit(1);
    }
    return texture;
}

void configurarTextura(GLuint texturaID) {
    glBindTexture(GL_TEXTURE_2D, texturaID);
    
    // Configurações de repetição de textura (wrap)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Envolve no eixo S
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Envolve no eixo T

    // Configurações de filtros de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtro para minificação
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtro para ampliação
}

void loadTextures() {
    sun.textura = loadTexture("assets/sun.jpg");
    mercury.textura = loadTexture("assets/mercury.jpg");
    venus.textura = loadTexture("assets/venus.jpg");
    earth.textura = loadTexture("assets/earth.jpg");
    mars.textura = loadTexture("assets/mars.jpg");
    jupiter.textura = loadTexture("assets/jupiter.jpg");
    saturn.textura = loadTexture("assets/saturn.jpg");
    uranus.textura = loadTexture("assets/uranus.jpg");
    neptune.textura = loadTexture("assets/neptune.jpg");
    backgroundTexture = loadTexture("assets/background.jpg");

    configurarTextura(sun.textura);
    configurarTextura(mercury.textura);
    configurarTextura(venus.textura);
    configurarTextura(earth.textura);
    configurarTextura(mars.textura);
    configurarTextura(jupiter.textura);
    configurarTextura(saturn.textura);
    configurarTextura(uranus.textura);
    configurarTextura(neptune.textura);
    configurarTextura(backgroundTexture);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);  // Habilitar texturas
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    loadTextures();  // Carregar as texturas
}

// Função para atualizar a direção da câmera baseado em seus ângulos
void updateCameraLookDirection() {
    cameraLookX = sin(cameraAngleH) * cos(cameraAngleV);
    cameraLookY = sin(cameraAngleV);
    cameraLookZ = -cos(cameraAngleH) * cos(cameraAngleV);
}

void drawOrbit(float distance) {
    glDisable(GL_LIGHTING);  // Desabilitar iluminação para desenhar a órbita
    glDisable(GL_TEXTURE_2D);  // Desabilitar texturas para a órbita
    glColor3f(0.2f, 0.2f, 0.2f);  // Define a cor branca para a órbita

    // Desenhar a órbita como um círculo
    glBegin(GL_LINE_LOOP);
    int numSegments = 500;  // Aumente o número para uma órbita mais suave
    for (int i = 0; i < numSegments; ++i) {
        float angle = 2.0f * M_PI * i / numSegments;
        float x = cos(angle) * distance;
        float z = sin(angle) * distance;
        glVertex3f(x, 0.0f, z);  // Mantém no plano XZ (horizontal)
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawBackground() {
    glPushMatrix();
    
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glDisable(GL_LIGHTING);  // Desabilitar iluminação para o fundo
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);  // Associar a textura de fundo
    glColor3f(1.0f, 1.0f, 1.0f);  // Branco para permitir a visualização da textura

    // Desenhar uma esfera grande ao redor da cena para o background
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, 200.0f, 50, 50);  // Tamanho grande da esfera para cobrir o universo
    gluDeleteQuadric(quadric);

    glEnable(GL_LIGHTING);  // Reabilitar a iluminação
    glPopMatrix();
}

void drawPlanet(const Astro& astro) {
    glPushMatrix();

    if(desenhaOrbita) drawOrbit(astro.distancia);
    
    glRotatef(astro.anguloTranslacao, 0.0f, 1.0f, 0.0f); // Rotação do planeta em torno do sol (translação)
    glTranslatef(astro.distancia, 0.0f, 0.0f); // Posição do planeta em relação ao Sol
    
    // Inclinação do eixo de rotação (em torno do eixo X)
    glRotatef(astro.inclinacaoEixo - 90, 1.0f, 0.0f, 0.0f);

    // Rotação em torno do próprio eixo
    glRotatef(astro.anguloRotacao, 0.0f, 0.0f, 1.0f);
    
    if (astro.textura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, astro.textura);  // Associar a textura
        glColor3f(1.0f, 1.0f, 1.0f);  // Branco para permitir a visualização da textura
    } else {
        glDisable(GL_TEXTURE_2D);  // Desabilitar texturas se não houver
    }
    
    // Desenhar a esfera com textura
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, astro.raio, 50, 50);
    gluDeleteQuadric(quadric);

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Definir a câmera com sua posição e a direção de onde está "olhando"
    gluLookAt(cameraX, cameraY, cameraZ, 
              cameraX + cameraLookX, cameraY + cameraLookY, cameraZ + cameraLookZ, 
              0.0, 1.0, 0.0);

    drawBackground();

    GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Configurar a emissão de luz do Sol para brilhar amarelo
    GLfloat emission[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    drawPlanet(sun); //Desenha o sol

    // Desativar emissão
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);

    // Desenhar os planetas
    // Distancia dos planetas em escala + compensação do raio do sol
    drawPlanet(mercury);
    drawPlanet(venus);
    drawPlanet(earth);
    drawPlanet(mars);
    drawPlanet(jupiter);
    drawPlanet(saturn);
    drawPlanet(uranus);
    drawPlanet(neptune);

    glutSwapBuffers();
}

void update(int value) {
    // Cada planeta se move a uma velocidade proporcional em relação a terra
    // Atualiza os ângulos dos planetas em órbita em relacao ao sol somente se não estiver pausado
    if (translacao) {
        mercury.anguloTranslacao += 4.1505681818*velOrbitalPadrao;
        venus.anguloTranslacao += 1.6233333333*velOrbitalPadrao;
        earth.anguloTranslacao += velOrbitalPadrao;
        mars.anguloTranslacao += 0.5316593886*velOrbitalPadrao;
        jupiter.anguloTranslacao += 0.0843144044*velOrbitalPadrao;
        saturn.anguloTranslacao += 0.0339483223*velOrbitalPadrao;
        uranus.anguloTranslacao += 0.0119125273*velOrbitalPadrao;
        neptune.anguloTranslacao += 0.0060669734*velOrbitalPadrao;
    }

    // Atualiza os ângulos de rotação dos planetas em torno do proprio eixo somente se não estiver pausado
    if (rotacao)
    {
        sun.anguloRotacao += 0.03703703704*velRotacaoPadrao;
        mercury.anguloRotacao += 0.01705146131*velRotacaoPadrao;
        venus.anguloRotacao += 0.004115056994*velRotacaoPadrao;
        earth.anguloRotacao += velRotacaoPadrao;
        mars.anguloRotacao += 0.9747072494*velRotacaoPadrao;
        jupiter.anguloRotacao += 2.4182037*velRotacaoPadrao;
        saturn.anguloRotacao += 2.345340536*velRotacaoPadrao;
        uranus.anguloRotacao += 1.392111369*velRotacaoPadrao;
        neptune.anguloRotacao += 1.489757914*velRotacaoPadrao;
    }
    
    // Atualiza a direção de visualização da câmera
    if (lookUp && cameraAngleV<1.5f) cameraAngleV += rotationSpeed;
    if (lookDown && cameraAngleV>-1.5f) cameraAngleV -= rotationSpeed;
    if (lookLeft) cameraAngleH -= rotationSpeed;
    if (lookRight) cameraAngleH += rotationSpeed;

    // Atualiza a posição da câmera com base na direção de visualização
    if (moveFoward) {
        cameraX += cameraLookX * movementSpeed;
        cameraZ += cameraLookZ * movementSpeed;
    }
    if (moveBackward) {
        cameraX -= cameraLookX * movementSpeed;
        cameraZ -= cameraLookZ * movementSpeed;
    }
    if (moveLeft) {
        cameraX += cameraLookZ * movementSpeed;
        cameraZ -= cameraLookX * movementSpeed;
    }
    if (moveRight) {
        cameraX -= cameraLookZ * movementSpeed;
        cameraZ += cameraLookX * movementSpeed;
    }
    if (moveUp) cameraY += movementSpeed;
    if (moveDown) cameraY -= movementSpeed;
    
    updateCameraLookDirection();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Função para capturar teclas de controle da câmera
// Precisa detectar a tecla pressionada e liberada
void movementKeys(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':  // Mover a câmera para a esquerda
            moveLeft = !moveLeft;
            break;
        case 'd':  // Mover a câmera para a direita
            moveRight = !moveRight;
            break;
        case 'w':  // Mover a câmera para frente
            moveFoward = !moveFoward;
            break;
        case 's':  // Mover a câmera para trás
            moveBackward = !moveBackward;
            break;
        case ' ':  // Mover a câmera para cima (eixo y)
            moveUp = !moveUp;
            break;
        case 'c':   // Mover a câmera para baixo (eixo y)
            moveDown = !moveDown;
            break;
        case 'h':  // Rotacionar para a esquerda (horizontalmente)
            lookLeft = !lookLeft;
            break;
        case 'k':  // Rotacionar para a direita (horizontalmente)
            lookRight = !lookRight;
            break;
        case 'u':  // Rotacionar para cima (verticalmente)
            lookUp = !lookUp;
            break;
        case 'j':  // Rotacionar para baixo (verticalmente)
            lookDown = !lookDown;
            break;
    }
    glutPostRedisplay();
}

// Função para capturar teclas de controle da câmera e configuração
void handleKeys(unsigned char key, int x, int y){
    movementKeys(key,x,y);

    switch (key) {
        case 'p':  // Pausar/retomar o movimento com a tecla P
            translacao = !translacao;
            break;
        case '1':   // Define a velocidade das órbitas
            velOrbitalPadrao = 0.5f;
            break;
        case '2':   // Define a velocidade das órbitas
            velOrbitalPadrao = 2.0f;
            break;
        case '3':   // Define a velocidade das órbitas
            velOrbitalPadrao = 4.0f;
            break;
        case 'r':   // Reposiciona a câmera na posição inicial
            cameraX = 2.97471;
            cameraY = 46.4001;
            cameraZ = 38.8908;
            cameraAngleH = 0.58;
            cameraAngleV = -0.8;
            break;
        case 'l':
            desenhaOrbita = !desenhaOrbita;
            break;
        case 27:   // Tecla ESC para sair
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / (float)height, 0.1, 400.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1660, 960);
    glutCreateWindow("Sistema Solar em OpenGL");

    init();
    loadTextures();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeys);        // Captura teclas pressionadas
    glutKeyboardUpFunc(movementKeys);     // Captura quando as teclas são liberadas
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}

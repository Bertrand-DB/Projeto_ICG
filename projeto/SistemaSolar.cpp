//Comando de compilação: g++ SistemaSolar.cpp -o sis -lGL -lGLU -lSOIL  -lGLEW -lglut -lm
#include <GL/glut.h>
#include <SOIL/SOIL.h>  // Biblioteca para carregar texturas
#include <cmath>
#include <cstdio>
#include <iostream>

// Ângulos de inclinação do eixo de rotação
#define INCLINACAO_EIXO_MERCURIO    0.01f
#define INCLINACAO_EIXO_VENUS       177.3f
#define INCLINACAO_EIXO_TERRA       23.26f
#define INCLINACAO_EIXO_MARTE       25.19f
#define INCLINACAO_EIXO_JUPITER     3.13f
#define INCLINACAO_EIXO_SATURNO     26.73f
#define INCLINACAO_EIXO_URANO       97.77f
#define INCLINACAO_EIXO_NETUNO      28.32f

#define DISTANCIA_PADRAO    3.0f
#define COMPENSACAO         20.0f
#define RAIO_PADRAO         0.1f

// Variável para armazenar as textura do sol e dos planetas
GLuint sunTexture;
GLuint mercuryTexture;
GLuint venusTexture;
GLuint earthTexture;
GLuint marsTexture;
GLuint jupiterTexture;
GLuint saturnTexture;
GLuint uranusTexture;
GLuint neptuneTexture;
GLuint backgroundTexture;

// Ângulos de translação (posição em órbita) dos planetas em graus
float mercuryTransAngle = 0.00f;
float venusTransAngle = 0.00f;
float earthTransAngle = 0.00f;
float marsTransAngle = 0.00f;
float jupiterTransAngle = 0.00f;
float saturnTransAngle = 0.00f;
float uranusTransAngle = 0.00f;
float neptuneTransAngle = 0.00f;

// Ângulos de rotação (no próprio eixo) dos planetas em graus
float sunRotAngle = 0.0f;
float mercuryRotAngle = 0.00f;
float venusRotAngle = 0.00f;
float earthRotAngle = 0.00f;
float marsRotAngle = 0.00f;
float jupiterRotAngle = 0.00f;
float saturnRotAngle = 0.00f;
float uranusRotAngle = 0.00f;
float neptuneRotAngle = 0.00f;

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

void loadTextures() {
    // Carregar a textura do Sol
    sunTexture = SOIL_load_OGL_texture("texturas/sun.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!sunTexture) {
        printf("Falha ao carregar a textura do Sol!\n");
        exit(1);
    }
    
    // Carregar as texturas dos planetas
    // Mercúrio
    mercuryTexture = SOIL_load_OGL_texture("texturas/mercury.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!mercuryTexture) {
        printf("Falha ao carregar a textura de Mercúrio!\n");
        exit(1);
    }

    // Vênus
    venusTexture = SOIL_load_OGL_texture("texturas/venus.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!venusTexture) {
        printf("Falha ao carregar a textura de Vênus!\n");
        exit(1);
    }
    
    // Terra
    earthTexture = SOIL_load_OGL_texture("texturas/earth.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!earthTexture) {
        printf("Falha ao carregar a textura da Terra!\n");
        exit(1);
    }
    
    // Marte
    marsTexture = SOIL_load_OGL_texture("texturas/mars.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!marsTexture) {
        printf("Falha ao carregar a textura de Marte!\n");
        exit(1);
    }
    
    // Júpiter
    jupiterTexture = SOIL_load_OGL_texture("texturas/jupiter.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!jupiterTexture) {
        printf("Falha ao carregar a textura de Júpiter!\n");
        exit(1);
    }

    // Saturno
    saturnTexture = SOIL_load_OGL_texture("texturas/saturn.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!saturnTexture) {
        printf("Falha ao carregar a textura de Saturno!\n");
        exit(1);
    }

    // Urano
    uranusTexture = SOIL_load_OGL_texture("texturas/uranus.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!uranusTexture) {
        printf("Falha ao carregar a textura de Urano!\n");
        exit(1);
    }

    // Neturno
    neptuneTexture = SOIL_load_OGL_texture("texturas/neptune.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!neptuneTexture) {
        printf("Falha ao carregar a textura de Netuno!\n");
        exit(1);
    }

    // Fundo
    backgroundTexture = SOIL_load_OGL_texture("texturas/background.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!backgroundTexture) {
        printf("Falha ao carregar a textura de fundo!\n");
        exit(1);
    }

    // Configurações da textura do Sol
    glBindTexture(GL_TEXTURE_2D, sunTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Configurações das texturas dos planetas
    // Mercúrio
    glBindTexture(GL_TEXTURE_2D, mercuryTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Vênus
    glBindTexture(GL_TEXTURE_2D, venusTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Terra
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Marte
    glBindTexture(GL_TEXTURE_2D, marsTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Júpiter
    glBindTexture(GL_TEXTURE_2D, jupiterTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Saturno
    glBindTexture(GL_TEXTURE_2D, saturnTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Urano
    glBindTexture(GL_TEXTURE_2D, uranusTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Netuno
    glBindTexture(GL_TEXTURE_2D, neptuneTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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


void drawPlanet(float distance, float size, float translationAngle, GLuint texture, float axialTilt, float rotationAngle) {
    glPushMatrix();

    if(desenhaOrbita) drawOrbit(distance);
    
    glRotatef(translationAngle, 0.0f, 1.0f, 0.0f); // Rotação do planeta em torno do sol (translação)
    glTranslatef(distance, 0.0f, 0.0f); // Posição do planeta em relação ao Sol
    
    // Inclinação do eixo de rotação (em torno do eixo X)
    glRotatef(axialTilt-90, 1.0f, 0.0f, 0.0f);

    // Rotação em torno do próprio eixo
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    
    if (texture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);  // Associar a textura
        glColor3f(1.0f, 1.0f, 1.0f);  // Branco para permitir a visualização da textura
    } else {
        glDisable(GL_TEXTURE_2D);  // Desabilitar texturas se não houver
    }
    
    // Desenhar a esfera com textura
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, size, 50, 50);
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

    drawPlanet(0.0f, 109*RAIO_PADRAO, 0.0f, sunTexture, 0.0f, sunRotAngle); //Desenha o sol

    // Desativar emissão
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);

    // Desenhar os planetas
    // Distancia dos planetas em escala + compensação do raio do sol
    drawPlanet(0.39*DISTANCIA_PADRAO+COMPENSACAO, 0.38*RAIO_PADRAO, mercuryTransAngle, mercuryTexture, INCLINACAO_EIXO_MERCURIO, mercuryRotAngle);
    drawPlanet(0.72*DISTANCIA_PADRAO+COMPENSACAO, 0.95*RAIO_PADRAO, venusTransAngle, venusTexture, INCLINACAO_EIXO_VENUS, venusRotAngle);
    drawPlanet(DISTANCIA_PADRAO+COMPENSACAO, RAIO_PADRAO, earthTransAngle, earthTexture, INCLINACAO_EIXO_TERRA, earthRotAngle);
    drawPlanet(1.52*DISTANCIA_PADRAO+COMPENSACAO, 0.53*RAIO_PADRAO, marsTransAngle, marsTexture, INCLINACAO_EIXO_MARTE, marsRotAngle);
    drawPlanet(5.2*DISTANCIA_PADRAO+COMPENSACAO, 11.21*RAIO_PADRAO, jupiterTransAngle, jupiterTexture, INCLINACAO_EIXO_JUPITER, jupiterRotAngle);
    drawPlanet(9.58*DISTANCIA_PADRAO+COMPENSACAO, 9.45*RAIO_PADRAO, saturnTransAngle, saturnTexture, INCLINACAO_EIXO_SATURNO, saturnRotAngle);
    drawPlanet(19.18*DISTANCIA_PADRAO+COMPENSACAO, 4.01*RAIO_PADRAO, uranusTransAngle, uranusTexture, INCLINACAO_EIXO_URANO, uranusRotAngle);
    drawPlanet(30.07*DISTANCIA_PADRAO+COMPENSACAO, 3.88*RAIO_PADRAO, neptuneTransAngle, neptuneTexture, INCLINACAO_EIXO_NETUNO, neptuneRotAngle);

    glutSwapBuffers();
}

void update(int value) {
    // Cada planeta se move a uma velocidade proporcional em relação a terra
    // Atualiza os ângulos dos planetas em órbita em relacao ao sol somente se não estiver pausado
    if (translacao) {
        mercuryTransAngle += 4.1505681818*velOrbitalPadrao;
        venusTransAngle += 1.6233333333*velOrbitalPadrao;
        earthTransAngle += velOrbitalPadrao;
        marsTransAngle += 0.5316593886*velOrbitalPadrao;
        jupiterTransAngle += 0.0843144044*velOrbitalPadrao;
        saturnTransAngle += 0.0339483223*velOrbitalPadrao;
        uranusTransAngle += 0.0119125273*velOrbitalPadrao;
        neptuneTransAngle += 0.0060669734*velOrbitalPadrao;
    }

    // Atualiza os ângulos de rotação dos planetas em torno do proprio eixo somente se não estiver pausado
    if (rotacao)
    {
        sunRotAngle += 0.03703703704*velRotacaoPadrao;
        mercuryRotAngle += 0.01705146131*velRotacaoPadrao;
        venusRotAngle += 0.004115056994*velRotacaoPadrao;
        earthRotAngle += velRotacaoPadrao;
        marsRotAngle += 0.9747072494*velRotacaoPadrao;
        jupiterRotAngle += 2.4182037*velRotacaoPadrao;
        saturnRotAngle += 2.345340536*velRotacaoPadrao;
        uranusRotAngle += 1.392111369*velRotacaoPadrao;
        neptuneRotAngle += 1.489757914*velRotacaoPadrao;
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

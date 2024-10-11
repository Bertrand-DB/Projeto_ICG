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

#define DISTANCIA_PADRAO 3.0f
#define RAIO_PADRAO 0.1f

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

// Ângulos de translação (posição em órbita) dos planetas em graus
float mercuryAngle = 0.00f;
float venusAngle = 0.00f;
float earthAngle = 0.00f;
float marsAngle = 0.00f;
float jupiterAngle = 0.00f;
float saturnAngle = 0.00f;
float uranusAngle = 0.00f;
float neptuneAngle = 0.00f;

float velOrbitalPadrao = 1.0f;

// Posição inicial da câmera
float cameraX = 0.0f, cameraY = 15.0f, cameraZ = 14.0f;
float cameraAngleH = 0.3f;  // Ângulo de rotação horizontal da câmera
float cameraAngleV = -0.8f;  // Ângulo de rotação vertical da câmera
float movementSpeed = 0.2f;  // Velocidade de movimento da câmera
float rotationSpeed = 0.02f;  // Velocidade de rotação da câmera

// Vetor direção da câmera
float cameraLookX = 0.0f, cameraLookY = 0.0f, cameraLookZ = -1.0f;

// Variável para pausar o movimento
bool isPaused = false;

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

void drawPlanet(float distance, float size, float angle, GLuint texture, float axialTilt = 0.0f) {
    glPushMatrix();
    
    glRotatef(angle, 0.0f, 1.0f, 0.0f); // Rotação do planeta em torno do sol (translação)
    glTranslatef(distance, 0.0f, 0.0f); // Posição do planeta em relação ao Sol
    
    // Inclinação do eixo de rotação (em torno do eixo X)
    glRotatef(axialTilt, 1.0f, 0.0f, 0.0f);
    
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

    GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Desenhar o Sol com textura
    drawPlanet(0.0f, 109*RAIO_PADRAO, 0.0f, sunTexture); 

    // Desenhar os planetas
    // Distancia dos planetas em escala + compensação do raio do sol
    drawPlanet(0.39*DISTANCIA_PADRAO+109*RAIO_PADRAO, 0.38*RAIO_PADRAO, mercuryAngle, mercuryTexture, INCLINACAO_EIXO_MERCURIO);
    drawPlanet(0.72*DISTANCIA_PADRAO+109*RAIO_PADRAO, 0.95*RAIO_PADRAO, venusAngle, venusTexture, INCLINACAO_EIXO_VENUS);
    drawPlanet(DISTANCIA_PADRAO+109*RAIO_PADRAO, RAIO_PADRAO, earthAngle, earthTexture, INCLINACAO_EIXO_TERRA);
    drawPlanet(1.52*DISTANCIA_PADRAO+109*RAIO_PADRAO, 0.53*RAIO_PADRAO, marsAngle, marsTexture, INCLINACAO_EIXO_MARTE);
    drawPlanet(5.2*DISTANCIA_PADRAO+109*RAIO_PADRAO, 11.21*RAIO_PADRAO, jupiterAngle, jupiterTexture, INCLINACAO_EIXO_JUPITER);
    drawPlanet(9.58*DISTANCIA_PADRAO+109*RAIO_PADRAO, 9.45*RAIO_PADRAO, saturnAngle, saturnTexture, INCLINACAO_EIXO_SATURNO);
    drawPlanet(19.18*DISTANCIA_PADRAO+109*RAIO_PADRAO, 4.01*RAIO_PADRAO, uranusAngle, uranusTexture, INCLINACAO_EIXO_URANO);
    drawPlanet(30.07*DISTANCIA_PADRAO+109*RAIO_PADRAO, 3.88*RAIO_PADRAO, neptuneAngle, neptuneTexture, INCLINACAO_EIXO_NETUNO);

    glutSwapBuffers();
}

void update(int value) {
    // Atualiza os ângulos dos planetas em órbita em relacao ao sol somente se não estiver pausado
    // Cada planeta se move a uma velocidade proporcional em relação a terra
    if (!isPaused) {
        mercuryAngle += 4.1505681818*velOrbitalPadrao;
        venusAngle += 1.6233333333*velOrbitalPadrao;
        earthAngle += velOrbitalPadrao;
        marsAngle += 0.5316593886*velOrbitalPadrao;
        jupiterAngle += 0.0843144044*velOrbitalPadrao;
        saturnAngle += 0.0339483223*velOrbitalPadrao;
        uranusAngle += 0.0119125273*velOrbitalPadrao;
        neptuneAngle += 0.0060669734*velOrbitalPadrao;
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

// Função para capturar teclas normais e controlar a câmera
void handleKeys(unsigned char key, int x, int y) {
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
        case 'p':  // Pausar/retomar o movimento com a tecla P
            isPaused = !isPaused;
            break;
        case '1':   // Define a velocidade das órbitas
            velOrbitalPadrao = 1.0f;
            break;
        case '2':   // Define a velocidade das órbitas
            velOrbitalPadrao = 3.0f;
            break;
        case '3':   // Define a velocidade das órbitas
            velOrbitalPadrao = 5.0f;
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
    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
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
    glutKeyboardUpFunc(handleKeys);     // Captura quando as teclas são liberadas
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}

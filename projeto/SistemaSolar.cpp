#include <GL/glut.h>
#include <cmath>
#include <iostream>

#define INCLINACAO_EIXO_MERCURIO    0.01f
#define INCLINACAO_EIXO_VENUS       177.3f
#define INCLINACAO_EIXO_TERRA       23.26f
#define INCLINACAO_EIXO_MARTE       25.19f
#define INCLINACAO_EIXO_JUPITER     3.13f
#define INCLINACAO_EIXO_SATURNO     26.73f
#define INCLINACAO_EIXO_URANO       97.77f
#define INCLINACAO_EIXO_NETUNO      28.32f

// Ângulos de translação (posição em órbita) dos planetas em graus
float mercuryAngle = 0.00f;
float venusAngle = 0.00f;
float earthAngle = 0.00f;
float marsAngle = 0.00f;
float jupiterAngle = 0.00f;
float saturnAngle = 0.00f;
float uranusAngle = 0.00f;
float neptuneAngle = 0.00f;

float velOrbitalTerra = 1.0f;

float cameraX = 0.0f, cameraY = 15.0f, cameraZ = 14.0f; // Posição inicial da câmera
float cameraAngleH = 0.0f;  // Ângulo de rotação horizontal da câmera
float cameraAngleV = 0.0f;  // Ângulo de rotação vertical da câmera
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

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Função para atualizar a direção da câmera baseado em seus ângulos
void updateCameraLookDirection() {
    cameraLookX = sin(cameraAngleH) * cos(cameraAngleV);
    cameraLookY = sin(cameraAngleV);
    cameraLookZ = -cos(cameraAngleH) * cos(cameraAngleV);
}

void drawPlanet(float distance, float size, float angle, float r, float g, float b, float axialTilt = 0.0f) {
    glPushMatrix();
    
    glRotatef(angle, 0.0f, 1.0f, 0.0f); // Rotação do planeta em torno do sol (translação)
    glTranslatef(distance, 0.0f, 0.0f); // Posição do planeta em relação ao Sol
    
    // Inclinação do eixo de rotação (em torno do eixo X)
    glRotatef(axialTilt, 1.0f, 0.0f, 0.0f);

    glColor3f(r, g, b);
    glutSolidSphere(size, 50, 50);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Definir a câmera com sua posição e a direção de onde está "olhando"
    gluLookAt(cameraX, cameraY, cameraZ, 
              cameraX + cameraLookX, cameraY + cameraLookY, cameraZ + cameraLookZ, 
              0.0, 1.0, 0.0);

    // Luz no centro do sistema solar
    GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Desenhar o Sol
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(1.0f, 50, 50);

    // Desenhar os planetas
    drawPlanet(2.0f, 0.1f, mercuryAngle, 0.5f, 0.5f, 0.5f, INCLINACAO_EIXO_MERCURIO);
    drawPlanet(2.5f, 0.2f, venusAngle, 0.9f, 0.6f, 0.3f, INCLINACAO_EIXO_VENUS);
    drawPlanet(3.0f, 0.3f, earthAngle, 0.2f, 0.5f, 1.0f, INCLINACAO_EIXO_TERRA);
    drawPlanet(4.0f, 0.2f, marsAngle, 1.0f, 0.4f, 0.3f, INCLINACAO_EIXO_MARTE);
    drawPlanet(6.0f, 0.7f, jupiterAngle, 1.0f, 0.8f, 0.4f, INCLINACAO_EIXO_JUPITER);
    drawPlanet(8.0f, 0.6f, saturnAngle, 0.9f, 0.7f, 0.4f, INCLINACAO_EIXO_SATURNO);
    drawPlanet(10.0f, 0.4f, uranusAngle, 0.4f, 0.7f, 1.0f, INCLINACAO_EIXO_URANO);
    drawPlanet(12.0f, 0.4f, neptuneAngle, 0.2f, 0.4f, 1.0f, INCLINACAO_EIXO_NETUNO);

    glutSwapBuffers();
}

void update(int value) {
    // Atualiza os ângulos dos planetas em órbita em relacao ao sol somente se não estiver pausado
    // Cada planeta se move a uma velocidade proporcional em relação a terra
    if (!isPaused) {
        mercuryAngle += 4.1505681818*velOrbitalTerra;
        venusAngle += 1.6233333333*velOrbitalTerra;
        earthAngle += velOrbitalTerra;
        marsAngle += 0.5316593886*velOrbitalTerra;
        jupiterAngle += 0.0843144044*velOrbitalTerra;
        saturnAngle += 0.0339483223*velOrbitalTerra;
        uranusAngle += 0.0119125273*velOrbitalTerra;
        neptuneAngle += 0.0060669734*velOrbitalTerra;
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
            velOrbitalTerra = 1.0f;
            break;
        case '2':   // Define a velocidade das órbitas
            velOrbitalTerra = 3.0f;
            break;
        case '3':   // Define a velocidade das órbitas
            velOrbitalTerra = 5.0f;
            break;
    }

    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Sistema Solar em OpenGL");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeys);        // Captura teclas pressionadas
    glutKeyboardUpFunc(handleKeys);     // Captura quando as teclas são liberadas
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}

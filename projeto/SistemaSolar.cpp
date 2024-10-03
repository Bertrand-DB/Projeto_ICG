#include <GL/glut.h>
#include <cmath>

// Ângulos de rotação dos planetas em graus
float mercuryAngle = 0.01f;   // Mercúrio
float venusAngle = 177.3f;    // Vênus
float earthAngle = 23.26f;     // Terra
float marsAngle = 25.19f;      // Marte
float jupiterAngle = 3.13f;    // Júpiter
float saturnAngle = 26.73f;    // Saturno
float uranusAngle = 97.77f;    // Urano
float neptuneAngle = 28.32f;   // Netuno

float cameraX = 0.0f, cameraY = 15.0f, cameraZ = 14.0f; // Posição inicial da câmera
float cameraAngleH = 0.0f;  // Ângulo de rotação horizontal da câmera
float cameraAngleV = 0.0f;  // Ângulo de rotação vertical da câmera
float movementSpeed = 0.2f;  // Velocidade de movimento da câmera
float rotationSpeed = 0.05f;  // Velocidade de rotação da câmera

// Vetor direção da câmera
float cameraLookX = 0.0f, cameraLookY = 0.0f, cameraLookZ = -1.0f;

// Variável para pausar o movimento
bool isPaused = false;

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

void drawPlanet(float distance, float size, float angle, float r, float g, float b) {
    glPushMatrix();
    glRotatef(angle, 0.0f, 1.0f, 0.0f); 
    glTranslatef(distance, 0.0f, 0.0f);
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
    drawPlanet(2.0f, 0.1f, mercuryAngle, 0.5f, 0.5f, 0.5f);
    drawPlanet(2.5f, 0.2f, venusAngle, 0.9f, 0.6f, 0.3f);
    drawPlanet(3.0f, 0.3f, earthAngle, 0.2f, 0.5f, 1.0f);
    drawPlanet(4.0f, 0.2f, marsAngle, 1.0f, 0.4f, 0.3f);
    drawPlanet(6.0f, 0.7f, jupiterAngle, 1.0f, 0.8f, 0.4f);
    drawPlanet(8.0f, 0.6f, saturnAngle, 0.9f, 0.7f, 0.4f);
    drawPlanet(10.0f, 0.4f, uranusAngle, 0.4f, 0.7f, 1.0f);
    drawPlanet(12.0f, 0.4f, neptuneAngle, 0.2f, 0.4f, 1.0f);

    glutSwapBuffers();
}

void update(int value) {
    if (!isPaused) { // Atualiza os ângulos somente se não estiver pausado
        mercuryAngle += 4.0f;
        venusAngle += 3.0f;
        earthAngle += 2.0f;
        marsAngle += 1.5f;
        jupiterAngle += 1.0f;
        saturnAngle += 0.8f;
        uranusAngle += 0.5f;
        neptuneAngle += 0.3f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Função para capturar teclas e controlar a câmera
void handleKeys(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':  // Mover para a esquerda (eixo X)
            cameraX -= movementSpeed;
            break;
        case 'd':  // Mover para a direita (eixo X)
            cameraX += movementSpeed;
            break;
        case 'w':  // Mover para frente (eixo Z)
            cameraZ -= movementSpeed;
            break;
        case 's':  // Mover para trás (eixo Z)
            cameraZ += movementSpeed;
            break;
        case 'q':  // Mover para cima (eixo Y)
            cameraY += movementSpeed;
            break;
        case 'e':  // Mover para baixo (eixo Y)
            cameraY -= movementSpeed;
            break;
        case 'h':  // Rotacionar para a esquerda (horizontalmente)
            cameraAngleH -= rotationSpeed;
            updateCameraLookDirection();
            break;
        case 'k':  // Rotacionar para a direita (horizontalmente)
            cameraAngleH += rotationSpeed;
            updateCameraLookDirection();
            break;
        case 'u':  // Rotacionar para cima (verticalmente)
            cameraAngleV += rotationSpeed;
            if (cameraAngleV > 1.5f) cameraAngleV = 1.5f;  // Limitar a rotação para não ultrapassar
            updateCameraLookDirection();
            break;
        case 'j':  // Rotacionar para baixo (verticalmente)
            cameraAngleV -= rotationSpeed;
            if (cameraAngleV < -1.5f) cameraAngleV = -1.5f; // Limitar para não ultrapassar
            updateCameraLookDirection();
            break;
        case ' ':  // Pausar/retomar o movimento com a tecla espaço
            isPaused = !isPaused;
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
    glutKeyboardFunc(handleKeys);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}

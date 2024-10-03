#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include "stb_image.h"

// Ângulos de rotação dos planetas em graus
float mercuryAngle = 0.01f;   // Mercúrio
float venusAngle = 177.3f;    // Vênus
float earthAngle = 23.26f;     // Terra
float marsAngle = 25.19f;      // Marte
float jupiterAngle = 3.13f;    // Júpiter
float saturnAngle = 26.73f;    // Saturno
float uranusAngle = 97.77f;    // Urano
float neptuneAngle = 28.32f;   // Netuno

// Texturas
GLuint textures[8];

// Posição inicial da câmera
float cameraX = 0.0f, cameraY = 15.0f, cameraZ = 14.0f;
float cameraAngleH = 0.0f;  // Ângulo de rotação horizontal da câmera
float cameraAngleV = 0.0f;  // Ângulo de rotação vertical da câmera
float movementSpeed = 0.2f;  // Velocidade de movimento da câmera
float rotationSpeed = 0.05f;  // Velocidade de rotação da câmera

// Vetor direção da câmera
float cameraLookX = 0.0f, cameraLookY = 0.0f, cameraLookZ = -1.0f;

// Variável para controlar se os planetas estão em movimento
bool isMoving = true;

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Configuração da luz
    GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Cor da luz
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor); // Luz difusa
}

GLuint loadTexture(const char* filename) {
    GLuint texture;
    int width, height, channels;
    
    // Carregar a imagem usando stb_image
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data == nullptr) {
        std::cout << "Erro ao carregar a textura: " << filename << std::endl;
        return 0;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Configurações da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Carregar a textura
    GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    // Gerar mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data); // Liberar a memória da imagem
    return texture;
}

void loadTextures() {
    textures[0] = loadTexture("texturas/mercury.jpg");
    textures[1] = loadTexture("texturas/venus.jpg");
    textures[2] = loadTexture("texturas/earth.jpg");
    textures[3] = loadTexture("texturas/mars.jpg");
    textures[4] = loadTexture("texturas/jupiter.jpg");
    textures[5] = loadTexture("texturas/saturn.jpg");
    textures[6] = loadTexture("texturas/uranus.jpg");
    textures[7] = loadTexture("texturas/neptune.jpg");
}

// Função para atualizar a direção da câmera baseado em seus ângulos
void updateCameraLookDirection() {
    cameraLookX = sin(cameraAngleH) * cos(cameraAngleV);
    cameraLookY = sin(cameraAngleV);
    cameraLookZ = -cos(cameraAngleH) * cos(cameraAngleV);
}

void drawPlanet(float distance, float size, float angle, GLuint texture) {
    glPushMatrix();
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(distance, 0.0f, 0.0f);
    
    // Ativar a textura
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Desenhar a esfera com textura
    glColor3f(1.0f, 1.0f, 1.0f); // Para garantir que a cor não interfira
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE); // Ativar textura para a quadrica
    gluSphere(quadric, size, 50, 50); // Criar a esfera
    gluDeleteQuadric(quadric); // Liberar memória
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Definir a câmera com sua posição e a direção de onde está "olhando"
    gluLookAt(cameraX, cameraY, cameraZ, 
              cameraX + cameraLookX, cameraY + cameraLookY, cameraZ + cameraLookZ, 
              0.0, 1.0, 0.0);

    // Desenhar o Sol
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(1.0f, 50, 50);

    // Desenhar os planetas com texturas
    drawPlanet(2.0f, 0.1f, mercuryAngle, textures[0]);
    drawPlanet(2.5f, 0.2f, venusAngle, textures[1]);
    drawPlanet(3.0f, 0.3f, earthAngle, textures[2]);
    drawPlanet(4.0f, 0.2f, marsAngle, textures[3]);
    drawPlanet(6.0f, 0.7f, jupiterAngle, textures[4]);
    drawPlanet(8.0f, 0.6f, saturnAngle, textures[5]);
    drawPlanet(10.0f, 0.4f, uranusAngle, textures[6]);
    drawPlanet(12.0f, 0.4f, neptuneAngle, textures[7]);

    glutSwapBuffers();
}

void update(int value) {
    if (isMoving) {  // Verifica se os planetas estão em movimento
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
            if (cameraAngleV > 1.5f) cameraAngleV = 1.5f;  // Limitar a rotação vertical
            updateCameraLookDirection();
            break;
        case 'j':  // Rotacionar para baixo (verticalmente)
            cameraAngleV -= rotationSpeed;
            if (cameraAngleV < -1.5f) cameraAngleV = -1.5f; // Limitar a rotação vertical
            updateCameraLookDirection();
            break;
        case 32:   // Tecla de espaço para pausar/reiniciar movimento
            isMoving = !isMoving;
            break;
        case 27:   // Tecla ESC para sair
            exit(0);
            break;
    }
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
    glutInitWindowSize(800, 600);
    glutCreateWindow("Sistema Solar em OpenGL");

    // Inicialize o GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Erro ao inicializar o GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    init();
    loadTextures();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeys);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}

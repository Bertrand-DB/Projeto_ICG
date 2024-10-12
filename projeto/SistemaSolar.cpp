//Comando de compilação: g++ SistemaSolar.cpp -o sis -lGL -lGLU -lSOIL -lglut -lm
#include <GL/glut.h>
#include <SOIL/SOIL.h>  // Biblioteca para carregar texturas
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

#define DISTANCIA_ESCALA    3.0f
#define COMPENSACAO         20.0f
#define RAIO_ESCALA         0.1f
#define SENSIBILIDADE       0.002f
#define SUN                 0
#define MERCURY             1
#define VENUS               2
#define EARTH               3
#define MARS                4
#define JUPITER             5
#define SATURN              6
#define URANUS              7
#define NEPTUNE             8
#define SATURNRING          9
#define SKY                 10

struct Planet {
    float distance;         // Distância do sol
    float radius;           // Raio do planeta
    float axialTilt;        // Inclinação do eixo de rotação
    float orbitSpeed;       // Velocidade orbital
    float rotationSpeed;    // Velocidade de rotação
    float orbitAngle;       // Ângulo atual na órbita
    float rotationAngle;    // Ângulo atual de rotação
    GLuint texture;         // ID da textura
};

struct Camera{
    float x;
    float y;
    float z;
    float angleH;
    float angleV;
};


Planet astros[11];

std::string texturePath[11] = {
    "texturas/sun.jpg",
    "texturas/mercury.jpg",
    "texturas/venus.jpg",
    "texturas/earth.jpg",
    "texturas/mars.jpg",
    "texturas/jupiter.jpg",
    "texturas/saturn.jpg",
    "texturas/uranus.jpg",
    "texturas/neptune.jpg",
    "texturas/saturnRing.png",
    "texturas/background.jpg"
};

float velOrbitalPadrao = 0.5f;
float velRotacaoPadrao = 1.0f;

// Posição espacial da câmera
float cameraX = 2.97471;
float cameraY = 10.2;
float cameraZ = 38.8908;

// Posição do mouse
int lastMouseX = 0;
int lastMouseY = 0;

// Posição angular da câmera
float cameraAngleH = 0.62;
float cameraAngleV = -0.3;

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


void loadTextures() {

    for (size_t i = 0; i <= 10; i++)
    {
        astros[i].texture = SOIL_load_OGL_texture(texturePath[i].c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
        if (!astros[i].texture) {
            printf("Falha ao carregar a textura %s !", texturePath[i].c_str());
            exit(1);
        }
        glBindTexture(GL_TEXTURE_2D, astros[i].texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

void initObjects(){

    //Sun
    astros[0].distance = 0.0f;
    astros[0].radius = 109*RAIO_ESCALA;
    astros[0].axialTilt = 0.0f;
    astros[0].orbitSpeed = 0.0f;
    astros[0].rotationSpeed = 0.03703703704*velRotacaoPadrao;
    astros[0].orbitAngle = 0.0f;
    astros[0].rotationAngle = 0.0f;

    //Mercury
    astros[1].distance = 0.39*DISTANCIA_ESCALA+COMPENSACAO;
    astros[1].radius = 0.38*RAIO_ESCALA;
    astros[1].axialTilt = 0.01f;
    astros[1].orbitSpeed = 4.1505681818*velOrbitalPadrao;
    astros[1].rotationSpeed = 0.01705146131*velRotacaoPadrao;
    astros[1].orbitAngle = 0.0f;
    astros[1].rotationAngle = 0.0f;

    //Venus
    astros[2].distance = 0.72*DISTANCIA_ESCALA+COMPENSACAO;
    astros[2].radius = 0.95*RAIO_ESCALA;
    astros[2].axialTilt = 177.3f;
    astros[2].orbitSpeed = 1.6233333333*velOrbitalPadrao;
    astros[2].rotationSpeed = 0.004115056994*velRotacaoPadrao;
    astros[2].orbitAngle = 0.0f;
    astros[2].rotationAngle = 0.0f;

    //Earth
    astros[3].distance = DISTANCIA_ESCALA+COMPENSACAO;
    astros[3].radius = RAIO_ESCALA;
    astros[3].axialTilt = 23.26f;
    astros[3].orbitSpeed = velOrbitalPadrao;
    astros[3].rotationSpeed = velRotacaoPadrao;
    astros[3].orbitAngle = 0.0f;
    astros[3].rotationAngle = 0.0f;

    //Mars
    astros[4].distance = 1.52*DISTANCIA_ESCALA+COMPENSACAO;
    astros[4].radius = 0.53*RAIO_ESCALA;
    astros[4].axialTilt = 25.19f;
    astros[4].orbitSpeed = 0.5316593886*velOrbitalPadrao;
    astros[4].rotationSpeed = 0.9747072494*velRotacaoPadrao;
    astros[4].orbitAngle = 0.0f;
    astros[4].rotationAngle = 0.0f;

    //Jupiter
    astros[5].distance = 5.2*DISTANCIA_ESCALA+COMPENSACAO;
    astros[5].radius = 11.21*RAIO_ESCALA;
    astros[5].axialTilt = 3.13f;
    astros[5].orbitSpeed = 0.0843144044*velOrbitalPadrao;
    astros[5].rotationSpeed = 2.4182037*velRotacaoPadrao;
    astros[5].orbitAngle = 0.0f;
    astros[5].rotationAngle = 0.0f;

    //Saturn
    astros[6].distance = 9.58*DISTANCIA_ESCALA+COMPENSACAO;
    astros[6].radius = 9.45*RAIO_ESCALA;
    astros[6].axialTilt = 26.73f;
    astros[6].orbitSpeed = 0.0339483223*velOrbitalPadrao;
    astros[6].rotationSpeed = 2.345340536*velRotacaoPadrao;
    astros[6].orbitAngle = 0.0f;
    astros[6].rotationAngle = 0.0f;

    //Uranus
    astros[7].distance = 19.18*DISTANCIA_ESCALA+COMPENSACAO;
    astros[7].radius = 4.01*RAIO_ESCALA;
    astros[7].axialTilt = 97.77f;
    astros[7].orbitSpeed = 0.0119125273*velOrbitalPadrao;
    astros[7].rotationSpeed = 1.392111369*velRotacaoPadrao;
    astros[7].orbitAngle = 0.0f;
    astros[7].rotationAngle = 0.0f;

    //Neptune
    astros[8].distance = 30.07*DISTANCIA_ESCALA+COMPENSACAO;
    astros[8].radius = 3.88*RAIO_ESCALA;
    astros[8].axialTilt = 28.32f;
    astros[8].orbitSpeed = 0.0060669734*velOrbitalPadrao;
    astros[8].rotationSpeed = 1.489757914*velRotacaoPadrao;
    astros[8].orbitAngle = 0.0f;
    astros[8].rotationAngle = 0.0f;

    //Saturn ring
    astros[9].distance = astros[6].distance;
    astros[9].radius = astros[6].radius;
    astros[9].axialTilt = 26.73f;
    astros[9].orbitSpeed = astros[6].orbitSpeed;
    astros[9].rotationSpeed = 0.0f;
    astros[9].orbitAngle = 0.0f;
    astros[9].rotationAngle = 0.0f;

    //Sky
    astros[10].distance = 0.0f;
    astros[10].radius = 200.0f;
    astros[10].axialTilt = 90.0f;
    astros[10].orbitSpeed = 0.0f;
    astros[10].rotationSpeed = 0.0f;
    astros[10].orbitAngle = 0.0f;
    astros[10].rotationAngle = 0.0f;
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);  // Habilitar texturas
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    loadTextures(); // Carregar as texturas
    initObjects();  // Inicia os valores dos objetos
}

bool cameraLocked = false; // Variável para controlar se a câmera está bloqueada no planeta
int focusedPlanet = 5; // Índice do planeta que está sendo focalizado

// Função de atualização da câmera
void updateCamera() {
    // Calcula a posição do planeta que está sendo seguido
    float planetZ = astros[focusedPlanet].distance * cos(astros[focusedPlanet].orbitAngle*0.0174533+1.4);
    float planetX = astros[focusedPlanet].distance * sin(astros[focusedPlanet].orbitAngle*0.0174533+1.4);
    //std::cout << "planeta: "<< planetX << "|" << planetZ << std::endl;
    // Atualiza a posição da câmera para estar um pouco acima do planeta
    cameraX = planetX;
    cameraY = 0.0f;
    cameraZ = planetZ; // Distância em relação ao planeta

    cameraAngleH = -astros[focusedPlanet].orbitAngle*0.0174533+0.1;
    
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
    glBindTexture(GL_TEXTURE_2D, astros[SKY].texture);  // Associar a textura de fundo
    glColor3f(1.0f, 1.0f, 1.0f);  // Branco para permitir a visualização da textura

    // Desenhar uma esfera grande ao redor da cena para o background
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, 200.0f, 50, 50);  // Tamanho grande da esfera para cobrir o universo
    gluDeleteQuadric(quadric);

    glEnable(GL_LIGHTING);  // Reabilitar a iluminação
    glPopMatrix();
}

void drawSaturnRing(){
    glPushMatrix();
    
    // Aplicar a textura ao anel
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, astros[SATURNRING].texture);

    // Ativar blending para melhorar a aparência do anel (textura com transparência)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Posicionar o anel no centro de Saturno
    glRotatef(astros[SATURNRING].orbitAngle, 0.0f, 1.0f, 0.0f); // Rotação do planeta em torno do sol (translação)
    glTranslatef(astros[SATURNRING].distance, 0.0f, 0.0f); // Posição do planeta em relação ao Sol
    glRotatef(-astros[SATURNRING].orbitAngle, 0.0f, 1.0f, 0.0f);
    
    // Inclinar o anel de acordo com a inclinação de Saturno
    glRotatef(astros[SATURNRING].axialTilt, 1.0f, 0.0f, 0.0f); // Inclinação no eixo X
    
    int segments = 300;
    float raioInt = 1.1161478728*astros[SATURNRING].radius;
    float raioExt = 2.327404261*astros[SATURNRING].radius;

    glBegin(GL_QUAD_STRIP);
    
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float x = cos(angle);
        float z = sin(angle);
        float sCoord = (float)i / (float)segments;

        glNormal3f(0.0f, 1.0f, 0.0f);  // Normal ajustada

        // Outer ring
        glTexCoord2f(sCoord, 1.0f); // Texture mapping
        glVertex3f(raioExt * x, 0.0f, raioExt * z);

        // Inner ring
        glTexCoord2f(sCoord, 0.0f); // Texture mapping
        glVertex3f(raioInt * x, 0.0f, raioInt * z);
    }
    glEnd();

    // Desativar blend e textura
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawPlanet(float distance, float size, float translationAngle, GLuint texture, float axialTilt, float rotationAngle) {
    glPushMatrix();

    if(desenhaOrbita) drawOrbit(distance);

    glRotatef(translationAngle, 0.0f, 1.0f, 0.0f);  // Rotação do planeta em torno do sol (translação)
    glTranslatef(distance, 0.0f, 0.0f);             // Posição do planeta em relação ao Sol
    glRotatef(-translationAngle, 0.0f, 1.0f, 0.0f); // Compensa a rotação da translação do planeta
    glRotatef(axialTilt-90, 1.0f, 0.0f, 0.0f);      // Inclinação do eixo de rotação (em torno do eixo X)
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);     // Rotação em torno do próprio eixo
    
    if (texture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);  // Associar a textura
        glColor3f(1.0f, 1.0f, 1.0f);            // Branco para permitir a visualização da textura
    } else {
        glDisable(GL_TEXTURE_2D);               // Desabilitar texturas se não houver
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

    drawPlanet(astros[SUN].distance, astros[SUN].radius, astros[SUN].orbitAngle, astros[SUN].texture, astros[SUN].axialTilt, astros[SUN].rotationAngle); //Desenha o sol

    // Desativar emissão
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);

    // Desenhar os planetas
    for (size_t i = 1; i <= 8; i++){
        drawPlanet(astros[i].distance, astros[i].radius, astros[i].orbitAngle, astros[i].texture, astros[i].axialTilt, astros[i].rotationAngle);
    }

    //desenha anel de saturno
    drawSaturnRing();

    glutSwapBuffers();
}

void update(int value) {
    // Cada planeta se move a uma velocidade proporcional em relação a terra
    // Atualiza os ângulos dos planetas em órbita em relacao ao sol somente se não estiver pausado
    if (translacao){
        for (size_t i = 1; i <= 9; i++){
           astros[i].orbitAngle += astros[i].orbitSpeed;
        }
    }

    // Atualiza os ângulos de rotação dos planetas em torno do proprio eixo somente se não estiver pausado
    if (rotacao){
        for (size_t i = 0; i <= 8; i++){
           astros[i].rotationAngle += astros[i].rotationSpeed;
        }
    }

    if(cameraLocked) updateCamera();

    if (!cameraLocked){    // Atualiza a posição da câmera com base na direção de visualização
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
    }
    
    updateCameraLookDirection();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Função para capturar o movimento do mouse
void mouseMotion(int x, int y) {
    int deltaX = x - lastMouseX;
    int deltaY = y - lastMouseY;

    // Atualizar ângulos da câmera com base no movimento do mouse
    cameraAngleH += deltaX * SENSIBILIDADE;
    cameraAngleV -= deltaY * SENSIBILIDADE;

    // Limitar a inclinação da câmera
    if (cameraAngleV > 1.5f) cameraAngleV = 1.5f; // Limite superior
    if (cameraAngleV < -1.5f) cameraAngleV = -1.5f; // Limite inferior

    lastMouseX = x;
    lastMouseY = y;
}

// Função para capturar o botão do mouse (opcional)
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Armazenar a posição do mouse quando o botão é pressionado
        lastMouseX = x;
        lastMouseY = y;
    }
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
    }
    glutPostRedisplay();
}

// Função para capturar teclas de controle da câmera e configuração
void handleKeys(unsigned char key, int x, int y){
    movementKeys(key,x,y);

    switch (key) {
        case 'p':  // Pausar/retomar o movimento de translação
            translacao = !translacao;
            break;
        case 'P':
            rotacao = !rotacao;
            break;
        case '6':
            cameraLocked = !cameraLocked;

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
            cameraY = 10.2;
            cameraZ = 38.8908;
            cameraAngleH = 0.62;
            cameraAngleV = -0.3;
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
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeys);        // Captura teclas pressionadas
    glutKeyboardUpFunc(movementKeys);     // Captura quando as teclas são liberadas
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}

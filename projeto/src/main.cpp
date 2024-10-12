//Comando de compilação: g++ src/main.cpp src/astro.cpp -o sis -lGL -lGLU -lSOIL  -lGLEW -lglut -lm

#include <cmath>
#include <cstdio>
#include <iostream>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include "../include/astro.h"

// Definições de constantes para a simulação de astros
#define DISTANCIA_PADRAO    3.0f       // Distância padrão para os astros em relação ao Sol
#define COMPENSACAO         20.0f      // Compensação para ajustar a posição dos astros
#define RAIO_PADRAO         0.1f       // Raio padrão para os astros

// Ângulos de inclinação do eixo de rotação
#define INCLINACAO_EIXO_MERCURIO    0.01f
#define INCLINACAO_EIXO_VENUS       177.3f
#define INCLINACAO_EIXO_TERRA       23.26f
#define INCLINACAO_EIXO_MARTE       25.19f
#define INCLINACAO_EIXO_JUPITER     3.13f
#define INCLINACAO_EIXO_SATURNO     26.73f
#define INCLINACAO_EIXO_URANO       97.77f
#define INCLINACAO_EIXO_NETUNO      28.32f

// Instanciando os astros com suas respectivas propriedades
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

float velOrbitalPadrao = 0.5f;  // Velocidade orbital padrão
float velRotacaoPadrao = 1.0f;   // Velocidade de rotação padrão

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

// Função para carregar uma textura a partir de um arquivo
GLuint loadTexture(const char* path) {
    GLuint texture = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (!texture) {
        std::cerr << "Falha ao carregar a textura: " << path << std::endl;
        exit(1); // Encerra o programa em caso de falha no carregamento
    }
    return texture; // Retorna o ID da textura carregada
}

// Função para configurar parâmetros de textura
void configurarTextura(GLuint texturaID) {
    glBindTexture(GL_TEXTURE_2D, texturaID); // Vincula a textura atual

    // Configurações de repetição de textura (wrap)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Envolve no eixo S
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Envolve no eixo T

    // Configurações de filtros de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtro para minificação
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtro para ampliação
}

// Função para carregar todas as texturas dos astros
void loadTextures() {
    // Carrega as texturas para cada astro e atribui aos atributos correspondentes
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

    // Configura as texturas para cada astro
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

// Função para inicializar as configurações OpenGL
void init() {
    glEnable(GL_DEPTH_TEST);   // Habilita o teste de profundidade
    glEnable(GL_LIGHTING);     // Habilita iluminação
    glEnable(GL_LIGHT0);       // Habilita a luz 0
    glEnable(GL_COLOR_MATERIAL); // Habilita materiais com base na cor
    glEnable(GL_TEXTURE_2D);   // Habilita texturas
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Define a cor de fundo da janela

    loadTextures();  // Carregar as texturas
}

// Função para atualizar a direção da câmera baseado em seus ângulos
void updateCameraLookDirection() {
    cameraLookX = sin(cameraAngleH) * cos(cameraAngleV); // Calcula a direção no eixo X
    cameraLookY = sin(cameraAngleV); // Calcula a direção no eixo Y
    cameraLookZ = -cos(cameraAngleH) * cos(cameraAngleV); // Calcula a direção no eixo Z
}

// Função para desenhar a órbita de um planeta
void drawOrbit(float distance) {
    glDisable(GL_LIGHTING);  // Desabilitar iluminação para desenhar a órbita
    glDisable(GL_TEXTURE_2D);  // Desabilitar texturas para a órbita
    glColor3f(0.2f, 0.2f, 0.2f);  // Define a cor cinza para a órbita

    // Desenhar a órbita como um círculo
    glBegin(GL_LINE_LOOP);
    int numSegments = 500;  // Aumente o número para uma órbita mais suave
    for (int i = 0; i < numSegments; ++i) {
        float angle = 2.0f * M_PI * i / numSegments; // Calcula o ângulo para cada segmento
        float x = cos(angle) * distance; // Calcula a coordenada X da órbita
        float z = sin(angle) * distance; // Calcula a coordenada Z da órbita
        glVertex3f(x, 0.0f, z);  // Mantém no plano XZ (horizontal)
    }
    glEnd();
    glEnable(GL_LIGHTING); // Reabilitar iluminação após desenhar a órbita
}

// Função para desenhar o fundo da cena
void drawBackground() {
    glPushMatrix(); // Salvar o estado atual da matriz

    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Rotaciona a matriz para que a esfera fique na posição correta
    glDisable(GL_LIGHTING);  // Desabilitar iluminação para o fundo
    glEnable(GL_TEXTURE_2D); // Habilitar texturas para o fundo
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);  // Associar a textura de fundo
    glColor3f(1.0f, 1.0f, 1.0f);  // Branco para permitir a visualização da textura

    // Desenhar uma esfera grande ao redor da cena para o background
    GLUquadric* quadric = gluNewQuadric(); // Criar um novo objeto quadrático
    gluQuadricTexture(quadric, GL_TRUE); // Habilitar texturização para o quadrático
    gluSphere(quadric, 200.0f, 50, 50);  // Tamanho grande da esfera para cobrir o universo
    gluDeleteQuadric(quadric); // Deletar o objeto quadrático após uso

    glEnable(GL_LIGHTING);  // Reabilitar a iluminação após desenhar o fundo
    glPopMatrix(); // Restaurar o estado da matriz anterior
}

// Função para desenhar um planeta com base na estrutura Astro
void drawPlanet(const Astro& astro) {
    glPushMatrix(); // Salvar o estado atual da matriz

    // Se a opção de desenhar órbitas estiver habilitada, desenha a órbita do planeta
    if(desenhaOrbita) drawOrbit(astro.distancia);
    
    // Rotação do planeta em torno do sol (translação)
    glRotatef(astro.anguloTranslacao, 0.0f, 1.0f, 0.0f); 
    glTranslatef(astro.distancia, 0.0f, 0.0f); // Posição do planeta em relação ao Sol
    
    // Inclinação do eixo de rotação (em torno do eixo X)
    glRotatef(astro.inclinacaoEixo - 90, 1.0f, 0.0f, 0.0f);

    // Rotação em torno do próprio eixo
    glRotatef(astro.anguloRotacao, 0.0f, 0.0f, 1.0f);
    
    // Se houver textura associada ao planeta, habilita a texturização
    if (astro.textura) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, astro.textura);  // Associar a textura do planeta
        glColor3f(1.0f, 1.0f, 1.0f);  // Branco para permitir a visualização da textura
    } else {
        glDisable(GL_TEXTURE_2D);  // Desabilitar texturas se não houver
    }
    
    // Desenhar a esfera do planeta com textura
    GLUquadric* quadric = gluNewQuadric(); // Criar um novo objeto quadrático
    gluQuadricTexture(quadric, GL_TRUE); // Habilitar texturização para o quadrático
    gluSphere(quadric, astro.raio, 50, 50); // Desenhar a esfera com o raio do planeta
    gluDeleteQuadric(quadric); // Deletar o objeto quadrático após uso

    glPopMatrix(); // Restaurar o estado da matriz anterior
}

// Função para exibir a cena
void display() {
    // Limpar os buffers de cor e profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); // Reseta a matriz de modelagem e visualização

    // Definir a câmera com sua posição e a direção de onde está "olhando"
    gluLookAt(cameraX, cameraY, cameraZ,  // Posição da câmera
              cameraX + cameraLookX, cameraY + cameraLookY, cameraZ + cameraLookZ, // Ponto para onde a câmera está olhando
              0.0, 1.0, 0.0); // Vetor de up (vertical)

    drawBackground(); // Desenhar o fundo da cena

    // Definir a posição da luz
    GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f}; // Posição da luz (no centro do sistema solar)
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos); // Aplicar a posição da luz

    // Configurar a emissão de luz do Sol para brilhar amarelo
    GLfloat emission[] = { 1.0f, 1.0f, 0.0f, 1.0f }; // Cor da emissão (amarelo)
    glMaterialfv(GL_FRONT, GL_EMISSION, emission); // Aplicar a emissão ao material do Sol

    drawPlanet(sun); // Desenhar o Sol

    // Desativar emissão após desenhar o Sol
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Sem emissão
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission); // Aplicar a configuração sem emissão

    // Desenhar os planetas em suas respectivas órbitas
    // A distância dos planetas é calculada em escala, com compensação pelo raio do Sol
    drawPlanet(mercury);
    drawPlanet(venus);
    drawPlanet(earth);
    drawPlanet(mars);
    drawPlanet(jupiter);
    drawPlanet(saturn);
    drawPlanet(uranus);
    drawPlanet(neptune);

    glutSwapBuffers(); // Trocar os buffers para exibir a cena renderizada
}

// Função para atualizar a animação
void update(int value) {
    // Atualiza os ângulos de translação dos planetas em relação ao Sol
    // Cada planeta se move a uma velocidade proporcional à Terra
    if (translacao) { // Verifica se a translação está ativada
        mercury.anguloTranslacao += 4.1505681818 * velOrbitalPadrao; // Atualiza o ângulo de translação do Mercúrio
        venus.anguloTranslacao += 1.6233333333 * velOrbitalPadrao; // Atualiza o ângulo de translação de Vênus
        earth.anguloTranslacao += velOrbitalPadrao; // Atualiza o ângulo de translação da Terra
        mars.anguloTranslacao += 0.5316593886 * velOrbitalPadrao; // Atualiza o ângulo de translação de Marte
        jupiter.anguloTranslacao += 0.0843144044 * velOrbitalPadrao; // Atualiza o ângulo de translação de Júpiter
        saturn.anguloTranslacao += 0.0339483223 * velOrbitalPadrao; // Atualiza o ângulo de translação de Saturno
        uranus.anguloTranslacao += 0.0119125273 * velOrbitalPadrao; // Atualiza o ângulo de translação de Urano
        neptune.anguloTranslacao += 0.0060669734 * velOrbitalPadrao; // Atualiza o ângulo de translação de Netuno
    }

    // Atualiza os ângulos de rotação dos planetas em torno do seu próprio eixo
    if (rotacao) { // Verifica se a rotação está ativada
        sun.anguloRotacao += 0.03703703704 * velRotacaoPadrao; // Atualiza o ângulo de rotação do Sol
        mercury.anguloRotacao += 0.01705146131 * velRotacaoPadrao; // Atualiza o ângulo de rotação do Mercúrio
        venus.anguloRotacao += 0.004115056994 * velRotacaoPadrao; // Atualiza o ângulo de rotação de Vênus
        earth.anguloRotacao += velRotacaoPadrao; // Atualiza o ângulo de rotação da Terra
        mars.anguloRotacao += 0.9747072494 * velRotacaoPadrao; // Atualiza o ângulo de rotação de Marte
        jupiter.anguloRotacao += 2.4182037 * velRotacaoPadrao; // Atualiza o ângulo de rotação de Júpiter
        saturn.anguloRotacao += 2.345340536 * velRotacaoPadrao; // Atualiza o ângulo de rotação de Saturno
        uranus.anguloRotacao += 1.392111369 * velRotacaoPadrao; // Atualiza o ângulo de rotação de Urano
        neptune.anguloRotacao += 1.489757914 * velRotacaoPadrao; // Atualiza o ângulo de rotação de Netuno
    }
    
    // Atualiza a direção de visualização da câmera baseado na entrada do usuário
    if (lookUp && cameraAngleV < 1.5f) cameraAngleV += rotationSpeed; // Inclinar a câmera para cima
    if (lookDown && cameraAngleV > -1.5f) cameraAngleV -= rotationSpeed; // Inclinar a câmera para baixo
    if (lookLeft) cameraAngleH -= rotationSpeed; // Girar a câmera para a esquerda
    if (lookRight) cameraAngleH += rotationSpeed; // Girar a câmera para a direita

    // Atualiza a posição da câmera com base na direção de visualização
    if (moveFoward) { // Se a tecla de mover para frente estiver pressionada
        cameraX += cameraLookX * movementSpeed; // Mover a câmera para frente na direção do olhar
        cameraZ += cameraLookZ * movementSpeed; // Mover a câmera para frente na direção do olhar
    }
    if (moveBackward) { // Se a tecla de mover para trás estiver pressionada
        cameraX -= cameraLookX * movementSpeed; // Mover a câmera para trás na direção oposta ao olhar
        cameraZ -= cameraLookZ * movementSpeed; // Mover a câmera para trás na direção oposta ao olhar
    }
    if (moveLeft) { // Se a tecla de mover para a esquerda estiver pressionada
        cameraX += cameraLookZ * movementSpeed; // Mover a câmera para a esquerda
        cameraZ -= cameraLookX * movementSpeed; // Mover a câmera para a esquerda
    }
    if (moveRight) { // Se a tecla de mover para a direita estiver pressionada
        cameraX -= cameraLookZ * movementSpeed; // Mover a câmera para a direita
        cameraZ += cameraLookX * movementSpeed; // Mover a câmera para a direita
    }
    if (moveUp) cameraY += movementSpeed; // Mover a câmera para cima
    if (moveDown) cameraY -= movementSpeed; // Mover a câmera para baixo
    
    // Atualiza a direção da câmera com base nos ângulos de visão
    updateCameraLookDirection();
    glutPostRedisplay(); // Solicitar uma nova exibição
    glutTimerFunc(16, update, 0); // Chamar a função de atualização novamente após 16 ms
}

// Função para capturar teclas de controle da câmera
// Precisa detectar a tecla pressionada e liberada
void movementKeys(unsigned char key, int x, int y) {
    // Verifica qual tecla foi pressionada e altera o estado das variáveis de movimento da câmera
    switch (key) {
        case 'a':  // Mover a câmera para a esquerda
            moveLeft = !moveLeft; // Alterna o estado do movimento para a esquerda
            break;
        case 'd':  // Mover a câmera para a direita
            moveRight = !moveRight; // Alterna o estado do movimento para a direita
            break;
        case 'w':  // Mover a câmera para frente
            moveFoward = !moveFoward; // Alterna o estado do movimento para frente
            break;
        case 's':  // Mover a câmera para trás
            moveBackward = !moveBackward; // Alterna o estado do movimento para trás
            break;
        case ' ':  // Mover a câmera para cima (eixo y)
            moveUp = !moveUp; // Alterna o estado do movimento para cima
            break;
        case 'c':   // Mover a câmera para baixo (eixo y)
            moveDown = !moveDown; // Alterna o estado do movimento para baixo
            break;
        case 'h':  // Rotacionar para a esquerda (horizontalmente)
            lookLeft = !lookLeft; // Alterna o estado da rotação para a esquerda
            break;
        case 'k':  // Rotacionar para a direita (horizontalmente)
            lookRight = !lookRight; // Alterna o estado da rotação para a direita
            break;
        case 'u':  // Rotacionar para cima (verticalmente)
            lookUp = !lookUp; // Alterna o estado da rotação para cima
            break;
        case 'j':  // Rotacionar para baixo (verticalmente)
            lookDown = !lookDown; // Alterna o estado da rotação para baixo
            break;
    }
    glutPostRedisplay(); // Solicita uma nova exibição após a mudança de estado
}

// Função para capturar teclas de controle da câmera e configuração
void handleKeys(unsigned char key, int x, int y){
    movementKeys(key, x, y); // Chama a função para lidar com as teclas de movimento

    // Verifica quais teclas adicionais foram pressionadas para ações específicas
    switch (key) {
        case 'p':  // Pausar/retomar o movimento com a tecla P
            translacao = !translacao; // Alterna o estado da translação (pausar ou retomar)
            break;
        case '1':   // Define a velocidade das órbitas
            velOrbitalPadrao = 0.5f; // Define a velocidade orbital padrão para 0.5
            break;
        case '2':   // Define a velocidade das órbitas
            velOrbitalPadrao = 2.0f; // Define a velocidade orbital padrão para 2.0
            break;
        case '3':   // Define a velocidade das órbitas
            velOrbitalPadrao = 4.0f; // Define a velocidade orbital padrão para 4.0
            break;
        case 'r':   // Reposiciona a câmera na posição inicial
            cameraX = 2.97471; // Define a posição inicial da câmera no eixo X
            cameraY = 46.4001; // Define a posição inicial da câmera no eixo Y
            cameraZ = 38.8908; // Define a posição inicial da câmera no eixo Z
            cameraAngleH = 0.58; // Define o ângulo horizontal inicial da câmera
            cameraAngleV = -0.8; // Define o ângulo vertical inicial da câmera
            break;
        case 'l': // Alternar a visualização da órbita
            desenhaOrbita = !desenhaOrbita; // Alterna o estado da visualização da órbita
            break;
        case 27:   // Tecla ESC para sair
            exit(0); // Encerra o programa
            break;
    }
    glutPostRedisplay(); // Solicita uma nova exibição após a mudança de estado
}

// Função para ajustar a visualização quando a janela é redimensionada
void reshape(int width, int height) {
    glViewport(0, 0, width, height); // Ajusta a área de visualização
    glMatrixMode(GL_PROJECTION); // Define a matriz de projeção
    glLoadIdentity(); // Reseta a matriz de projeção
    gluPerspective(45.0, (float)width / (float)height, 0.1, 400.0); // Define a perspectiva
    glMatrixMode(GL_MODELVIEW); // Retorna à matriz de modelo e visualização
}

// Função principal do programa
int main(int argc, char** argv) {
    glutInit(&argc, argv); // Inicializa o GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Configura o modo de exibição
    glutInitWindowSize(1660, 960); // Define o tamanho da janela
    glutCreateWindow("Sistema Solar em OpenGL"); // Cria a janela com o título especificado

    init(); // Inicializa as configurações do OpenGL
    loadTextures(); // Carrega as texturas necessárias
    glutDisplayFunc(display); // Define a função de exibição
    glutReshapeFunc(reshape); // Define a função de redimensionamento da janela
    glutKeyboardFunc(handleKeys); // Captura teclas pressionadas
    glutKeyboardUpFunc(movementKeys); // Captura quando as teclas são liberadas
    glutTimerFunc(25, update, 0); // Configura o temporizador para atualizações regulares

    glutMainLoop(); // Inicia o loop principal do GLUT
    return 0; // Retorna 0 ao sistema operacional
}

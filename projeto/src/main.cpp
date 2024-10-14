//Comando de compilação: g++ src/main.cpp src/menu.cpp src/astro.cpp src/camera.cpp src/textura.cpp -o sis -lGL -lGLU -lSOIL -lglut -lm
#include <cmath>
#include <cstdio>
#include <iostream>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

#include "../include/menu.h"
#include "../include/astro.h"
#include "../include/camera.h"
#include "../include/textura.h"

// Definições de constantes para a simulação de astros
#define DISTANCIA_PADRAO    3.0f       // Distância padrão para os astros em relação ao Sol
#define COMPENSACAO         20.0f      // Compensação para ajustar a posição dos astros
#define RAIO_PADRAO         0.1f       // Raio padrão para os astros
#define SENSIBILIDADE       0.002f
#define VEL_ORBITAL_PADRAO  0.3f
#define VEL_ROTACAO_PADRAO  2.0f
// Para uma escala real de tempo/velocidade, a VEL_ROTACAO_PADRAO deve ser 365.25*VEL_ORBITAL_PADRAO
// Se VEL_ROTACAO_PADRAO = 2.0 -> VEL_ORBITAL_PADRAO = 0.00547570157

// Instanciando os astros com suas respectivas propriedades
// Astro(distancia, raio, inclinacaoEixo, anguloRotacao, anguloTranslacao, textura)
Astro sun(0.0f, 109*RAIO_PADRAO, 0.03703703704*VEL_ROTACAO_PADRAO, 0.0f, 0.0f, 0.00f, 0.00f, 0);
Astro mercury(0.39*DISTANCIA_PADRAO+COMPENSACAO, 0.38*RAIO_PADRAO, 0.01f, 0.01705146131*VEL_ROTACAO_PADRAO, 4.1505681818*VEL_ORBITAL_PADRAO, 0.0f, 0.0f, 0);
Astro venus(0.72 * DISTANCIA_PADRAO + COMPENSACAO, 0.95 * RAIO_PADRAO, 177.3f, 0.004115056994*VEL_ROTACAO_PADRAO, 1.6233333333*VEL_ORBITAL_PADRAO, 0.0f, 0.0f, 0);
Astro earth(DISTANCIA_PADRAO + COMPENSACAO, RAIO_PADRAO, 23.26f, VEL_ROTACAO_PADRAO, VEL_ORBITAL_PADRAO, 0.0f, 0.0f, 0);
Astro mars(1.52*DISTANCIA_PADRAO+COMPENSACAO, 0.53*RAIO_PADRAO, 25.19f, 0.9747072494*VEL_ROTACAO_PADRAO, 0.5316593886*VEL_ORBITAL_PADRAO, 0.0f, 0.0f, 0);
Astro jupiter(5.2*DISTANCIA_PADRAO+COMPENSACAO, 11.21*RAIO_PADRAO, 3.13f, 2.4182037*VEL_ROTACAO_PADRAO, 0.0843144044*VEL_ORBITAL_PADRAO, 0.0f, 0.0f, 0);
Astro saturn(9.58*DISTANCIA_PADRAO+COMPENSACAO, 9.45*RAIO_PADRAO, 26.73f, 2.345340536*VEL_ROTACAO_PADRAO, 0.0339483223*VEL_ORBITAL_PADRAO, 0.0f, 0.0f, 0);
Astro uranus(19.18*DISTANCIA_PADRAO+COMPENSACAO, 4.01*RAIO_PADRAO, 97.77f, 1.392111369*VEL_ROTACAO_PADRAO, 0.0119125273*VEL_ORBITAL_PADRAO, 0.0f, 0.0f, 0);
Astro neptune(30.07*DISTANCIA_PADRAO+COMPENSACAO, 3.88*RAIO_PADRAO, 28.32f, 1.489757914*VEL_ROTACAO_PADRAO, 0.0060669734*VEL_ORBITAL_PADRAO, 0.0f, 0.0f, 0);
Astro moon(0.00257*DISTANCIA_PADRAO, 0.2721072437*RAIO_PADRAO, 0.0f, 0.0f, 13.36873382*VEL_ORBITAL_PADRAO, 0.0f, 0.0f, 0);

GLuint backgroundTexture;
GLuint saturnRingTexture;
GLuint controle_menu_tex, mercury_info_tex, venus_info_tex, earth_info_tex, mars_info_tex, jupiter_info_tex, saturn_info_tex, uranus_info_tex, neptune_info_tex;

Camera camera(2.97471f, 46.4001f, 38.8908f, 0.0f, 0.0f, -1.0f, 0.58, -0.8, 0.1f, 0.02f);

// Variável para pausar o movimento de translação
bool translacao = true;
bool rotacao = true;
bool desenhaOrbita = false;
bool showInfo = true;
int cameraLocked = 0;

Astro* target;
GLuint* target_info_tex = &controle_menu_tex;
float espacamentoAngular = 88.3;

// Variáveis para controlar as teclas de movimento
bool moveUp = false;
bool moveDown = false;
bool moveLeft = false;
bool moveRight = false;
bool moveFoward = false;
bool moveBackward = false;
int lastMouseX;
int lastMouseY;

// Função para carregar todas as texturas dos astros
void loadTextures() {
    // Carrega as texturas para cada astro e atribui aos atributos correspondentes
    sun.set_textura(loadTexture("assets/sun.jpg"));
    mercury.set_textura(loadTexture("assets/mercury.jpg"));
    venus.set_textura(loadTexture("assets/venus.jpg"));
    earth.set_textura(loadTexture("assets/earth.jpg"));
    mars.set_textura(loadTexture("assets/mars.jpg"));
    jupiter.set_textura(loadTexture("assets/jupiter.jpg"));
    saturn.set_textura(loadTexture("assets/saturn.jpg"));
    saturnRingTexture = loadTexture("assets/saturnRing.png");
    uranus.set_textura(loadTexture("assets/uranus.jpg"));
    neptune.set_textura(loadTexture("assets/neptune.jpg"));
    backgroundTexture = loadTexture("assets/background.jpg");
    moon.set_textura(loadTexture("assets/moon.jpg"));
    
    mercury_info_tex = loadTexture("assets/mercury_info.png");
    venus_info_tex = loadTexture("assets/venus_info.png");
    earth_info_tex = loadTexture("assets/earth_info.png");
    mars_info_tex = loadTexture("assets/mars_info.png");
    jupiter_info_tex = loadTexture("assets/jupiter_info.png");
    saturn_info_tex = loadTexture("assets/saturn_info.png");
    uranus_info_tex = loadTexture("assets/uranus_info.png");
    neptune_info_tex = loadTexture("assets/neptune_info.png");
    controle_menu_tex = loadTexture("assets/controle_menu_info.png");

    // Configura as texturas para cada astro
    configurarTextura(sun.get_textura());
    configurarTextura(mercury.get_textura());
    configurarTextura(venus.get_textura());
    configurarTextura(earth.get_textura());
    configurarTextura(mars.get_textura());
    configurarTextura(jupiter.get_textura());
    configurarTextura(saturn.get_textura());
    configurarTextura(saturnRingTexture);
    configurarTextura(uranus.get_textura());
    configurarTextura(neptune.get_textura());
    configurarTextura(backgroundTexture);
    configurarTextura(moon.get_textura());

    configurarTextura(mercury_info_tex);
    configurarTextura(venus_info_tex);
    configurarTextura(earth_info_tex);
    configurarTextura(mars_info_tex);
    configurarTextura(jupiter_info_tex);
    configurarTextura(saturn_info_tex);
    configurarTextura(uranus_info_tex);
    configurarTextura(neptune_info_tex);
    configurarTextura(controle_menu_tex);
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

void renderInfo(GLuint* textureId, float x, float y, float width, float height) {
    // Salvar a matriz de projeção e a matriz de visualização atuais
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Configurar uma projeção ortográfica (sem perspectiva)
    int screenWidth = glutGet(GLUT_WINDOW_WIDTH);  // Defina a largura da sua tela
    int screenHeight = glutGet(GLUT_WINDOW_HEIGHT); // Defina a altura da sua tela
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

    // Trocar para a matriz de modelo
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Habilitar o uso de texturas 2D
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    // Ativar o blending para lidar com transparência (PNG, por exemplo)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Associar a textura
    glBindTexture(GL_TEXTURE_2D, *textureId);

    // Renderizar o quad com a textura aplicada
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);                   // Canto inferior esquerdo
    glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y);           // Canto inferior direito
    glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y + height);  // Canto superior direito
    glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + height);          // Canto superior esquerdo
    glEnd();

    // Desabilitar texturas e blending
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    // Restaurar a matriz de visualização e projeção anteriores
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  // Voltar para o modo de modelo
}

void follow() {
    // Calcula a posição do planeta que está sendo seguido
    //graus para radianos (*0.0174533)
    //k graus de espaçamento do planeta

    camera.set_posZ(target->get_distancia() * cos((target->get_anguloTranslacao()+espacamentoAngular)*0.0174533));
    camera.set_posX(target->get_distancia() * sin((target->get_anguloTranslacao()+espacamentoAngular)*0.0174533));
    camera.set_posY(0.0);

    // Compensa a rotação de translação na angulação da camera
    camera.set_angleH(-target->get_anguloTranslacao()*0.0174533-0.1);
    camera.set_angleV(0.0);
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

void drawSaturnRing(){
    glPushMatrix();
    
    // Aplicar a textura ao anel
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, saturnRingTexture);

    // Ativar blending para melhorar a aparência do anel (se a textura tiver transparência)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Posicionar o anel no centro de Saturno
    glRotatef(saturn.get_anguloTranslacao(), 0.0f, 1.0f, 0.0f); // Rotação do planeta em torno do sol (translação)
    glTranslatef(saturn.get_distancia(), 0.0f, 0.0f); // Posição do planeta em relação ao Sol
    glRotatef(-saturn.get_anguloTranslacao(), 0.0f, 1.0f, 0.0f);
    
    // Inclinar o anel de acordo com a inclinação de Saturno
    glRotatef(saturn.get_inclinacaoEixo(), 1.0f, 0.0f, 0.0f); // Inclinação no eixo X
    
    int segments = 300;
    float raioInt = 1.1161478728*saturn.get_raio();
    float raioExt = 2.327404261*saturn.get_raio();

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

void drawMoon(){
    glPushMatrix();

    glRotatef(earth.get_anguloTranslacao(), 0.0f, 1.0f, 0.0f);  // Rotação de translação da Terra ao redor do Sol
    glTranslatef(earth.get_distancia(), 0.0f, 0.0f);       // Translação da Terra para sua órbita
    glRotatef(-earth.get_anguloTranslacao(), 0.0f, 1.0f, 0.0f); // Compensação da Rotação de translação da Terra
    glRotatef(moon.get_anguloTranslacao(), 0.0f, 1.0f, 0.0f);   // Rotação de translação da lua ao redor da terra

    float anguloOrbitalZ = 5.2*sin(moon.get_anguloTranslacao()*(M_PI/180));
    glRotatef(anguloOrbitalZ, 0.0f, 0.0f, 1.0f);   // Rotação de translação da lua pela terra
    glTranslatef((moon.get_raio()+earth.get_raio()+moon.get_distancia())*DISTANCIA_PADRAO, 0.0f, 0.0f);        // Posiciona a lua na órbita da terra
    
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);                      // ângulo de correção de textura
    
    if (moon.get_textura()) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, moon.get_textura()); // Associar a textura
        glColor3f(1.0f, 1.0f, 1.0f);                        // Branco para permitir a visualização da textura
    } else {
        glColor3f(1.0f, 0.0f, 0.0f);
        glDisable(GL_TEXTURE_2D);                           // Desabilitar texturas se não houver
    }
    
    // Desenhar a esfera com textura
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, moon.get_raio(), 50, 50);
    gluDeleteQuadric(quadric);

    glPopMatrix();
}

// Função para desenhar um planeta com base na estrutura Astro
void drawPlanet(Astro& astro) {
    glPushMatrix(); // Salvar o estado atual da matriz

    // Se a opção de desenhar órbitas estiver habilitada, desenha a órbita do planeta
    if(desenhaOrbita) drawOrbit(astro.get_distancia());
    
    // Rotação do planeta em torno do sol (translação)
    glRotatef(astro.get_anguloTranslacao(), 0.0f, 1.0f, 0.0f); 
    glTranslatef(astro.get_distancia(), 0.0f, 0.0f); // Posição do planeta em relação ao Sol
    glRotatef(-astro.get_anguloTranslacao(), 0.0f, 1.0f, 0.0f); 

    // Inclinação do eixo de rotação (em torno do eixo X)
    glRotatef(astro.get_inclinacaoEixo() - 90, 1.0f, 0.0f, 0.0f);

    // Rotação em torno do próprio eixo
    glRotatef(astro.get_anguloRotacao(), 0.0f, 0.0f, 1.0f);
    
    // Se houver textura associada ao planeta, habilita a texturização
    if (astro.get_textura()) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, astro.get_textura());  // Associar a textura do planeta
        glColor3f(1.0f, 1.0f, 1.0f);  // Branco para permitir a visualização da textura
    } else {
        glDisable(GL_TEXTURE_2D);  // Desabilitar texturas se não houver
    }
    
    // Desenhar a esfera do planeta com textura
    GLUquadric* quadric = gluNewQuadric(); // Criar um novo objeto quadrático
    gluQuadricTexture(quadric, GL_TRUE); // Habilitar texturização para o quadrático
    gluSphere(quadric, astro.get_raio(), 50, 50); // Desenhar a esfera com o raio do planeta
    gluDeleteQuadric(quadric); // Deletar o objeto quadrático após uso

    glPopMatrix(); // Restaurar o estado da matriz anterior
}

// Função para exibir a cena
void display() {
    // Limpar os buffers de cor e profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (showControlersInfo == true && !cameraLocked) {
        renderInfo(&controle_menu_tex, 500, 300, 760, 517);
    } else {
        renderMenu();
    }

    glLoadIdentity(); // Reseta a matriz de modelagem e visualização

    // Definir a câmera com sua posição e a direção de onde está "olhando"
    camera.applyView();

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

    //desenha anel de saturno
    drawSaturnRing();
    drawMoon();

    if(cameraLocked && showInfo && !showControlersInfo){
        renderInfo(target_info_tex, 100, 160, 650, 700);
    }

    glutSwapBuffers(); // Trocar os buffers para exibir a cena renderizada
}

// Função para atualizar a animação
void update(int value) {
    // Atualiza os ângulos de translação dos planetas em relação ao Sol
    // Cada planeta se move a uma velocidade proporcional à Terra
    if (translacao) { // Verifica se a translação está ativada
        mercury.update_anguloTranslacao();  // Atualiza o ângulo de translação do Mercúrio
        venus.update_anguloTranslacao();    // Atualiza o ângulo de translação de Vênus
        earth.update_anguloTranslacao();    // Atualiza o ângulo de translação da Terra
        mars.update_anguloTranslacao();     // Atualiza o ângulo de translação de Marte
        jupiter.update_anguloTranslacao();  // Atualiza o ângulo de translação de Júpiter
        saturn.update_anguloTranslacao();   // Atualiza o ângulo de translação de Saturno
        uranus.update_anguloTranslacao();   // Atualiza o ângulo de translação de Urano
        neptune.update_anguloTranslacao();  // Atualiza o ângulo de translação de Netuno
        moon.update_anguloTranslacao();
    }

    // Atualiza os ângulos de rotação dos planetas em torno do seu próprio eixo
    if (rotacao) { // Verifica se a rotação está ativada
        sun.update_anguloRotacao();     // Atualiza o ângulo de rotação do Sol
        mercury.update_anguloRotacao(); // Atualiza o ângulo de rotação do Mercúrio
        venus.update_anguloRotacao();   // Atualiza o ângulo de rotação de Vênus
        earth.update_anguloRotacao();   // Atualiza o ângulo de rotação da Terra
        mars.update_anguloRotacao();    // Atualiza o ângulo de rotação de Marte
        jupiter.update_anguloRotacao(); // Atualiza o ângulo de rotação de Júpiter
        saturn.update_anguloRotacao();  // Atualiza o ângulo de rotação de Saturno
        uranus.update_anguloRotacao();  // Atualiza o ângulo de rotação de Urano
        neptune.update_anguloRotacao(); // Atualiza o ângulo de rotação de Netuno
    }

    if (cameraLocked) follow();
    else{                                       // Atualiza a posição da câmera com base na direção de visualização
        if (moveFoward) camera.moveForward();       // Se a tecla de mover para frente estiver pressionada
        if (moveBackward) camera.moveBackward();    // Se a tecla de mover para trás estiver pressionada
        if (moveLeft) camera.moveLeft();            // Se a tecla de mover para a esquerda estiver pressionada
        if (moveRight) camera.moveRight();          // Se a tecla de mover para a direita estiver pressionada
        if (moveUp) camera.moveUp();              // Mover a câmera para cima
        if (moveDown) camera.moveDown();          // Mover a câmera para baixo
    }
    // Atualiza a direção da câmera com base nos ângulos de visão
    camera.updateLookDirection();
    glutPostRedisplay();            // Solicitar uma nova exibição
    glutTimerFunc(16, update, 0);   // Chamar a função de atualização novamente após 16 ms
}

// Função para capturar o movimento do mouse
void mouseMotion(int x, int y) {
    int deltaX = x - lastMouseX;
    int deltaY = y - lastMouseY;

    // Atualizar ângulos da câmera com base no movimento do mouse
    camera.set_angleH(camera.get_angleH() + deltaX*SENSIBILIDADE);
    camera.set_angleV(camera.get_angleV() - deltaY*SENSIBILIDADE);

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

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP: // Sobe no menu
            selectedItem++;
            if (selectedItem > 1) selectedItem = 1;
            break;
        case GLUT_KEY_DOWN: // Desce no menu
            selectedItem--;
            if (selectedItem < 0) selectedItem = 0;
            break;
    }

    glutPostRedisplay();
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
    }
    glutPostRedisplay(); // Solicita uma nova exibição após a mudança de estado
}

// Função para capturar teclas de controle da câmera e configuração
void handleKeys(unsigned char key, int x, int y){
    movementKeys(key, x, y); // Chama a função para lidar com as teclas de movimento

    if ('1' <= key && '8' >= key){
        int num = key-'0';
        showControlersInfo = false;
        if(cameraLocked == num) cameraLocked = 0;
        else cameraLocked = num;
    }

    // Verifica quais teclas adicionais foram pressionadas para ações específicas
    switch (key) {
        case '1':
            target = &mercury;
            target_info_tex = &mercury_info_tex;
            espacamentoAngular = 88.3;
            break;
        case '2':
            target = &venus;
            target_info_tex = &venus_info_tex;
            espacamentoAngular = 88.3;
            break;
        case '3':
            target = &earth;
            target_info_tex = &earth_info_tex;
            espacamentoAngular = 88.3;
            break;
        case '4':
            target = &mars;
            target_info_tex = &mars_info_tex;
            espacamentoAngular = 88.3;
            break;
        case '5':
            target = &jupiter;
            target_info_tex = &jupiter_info_tex;
            espacamentoAngular = 82.6;
            break;
        case '6':
            target = &saturn;
            target_info_tex = &saturn_info_tex;
            espacamentoAngular = 82.6;
            break;
        case '7':
            target = &uranus;
            target_info_tex = &uranus_info_tex;
            espacamentoAngular = 87.4;
            break;
        case '8':
            target = &neptune;
            target_info_tex = &neptune_info_tex;
            espacamentoAngular = 87.8;
            break;
        case 'p':  // Pausar/retomar o movimento com a tecla P
            translacao = !translacao; // Alterna o estado da translação (pausar ou retomar)
            break;
        case 'P':
            rotacao = !rotacao;
            break;
        case 'i':
            showInfo = !showInfo;
            break;
        case 'r':   // Reposiciona a câmera na posição inicial
            camera.set_posX(2.97471); // Define a posição inicial da câmera no eixo X
            camera.set_posY(10.2); // Define a posição inicial da câmera no eixo Y
            camera.set_posZ(38.8908); // Define a posição inicial da câmera no eixo Z
            camera.set_angleH(0.62); // Define o ângulo horizontal inicial da câmera
            camera.set_angleV(-0.3); // Define o ângulo vertical inicial da câmera
            break;
        case 'l': // Alternar a visualização da órbita
            desenhaOrbita = !desenhaOrbita; // Alterna o estado da visualização da órbita
            break;
        case 27:
            showControlersInfo = false;
            break;
        case 13: // Enter
            if (selectedItem == 0) {
                exit(0);
            } else if (selectedItem == 1) {
                showControlersInfo = true;
                cameraLocked = false;
            }
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
    glutDisplayFunc(display); // Define a função de exibição
    glutReshapeFunc(reshape); // Define a função de redimensionamento da janela
    glutKeyboardFunc(handleKeys); // Captura teclas pressionadas
    glutKeyboardUpFunc(movementKeys); // Captura quando as teclas são liberadas
    glutSpecialFunc(specialKeys);       //Captura as teclas especias como as setinhas do teclado
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(25, update, 0); // Configura o temporizador para atualizações regulares

    glutMainLoop(); // Inicia o loop principal do GLUT
    return 0; // Retorna 0 ao sistema operacional
}

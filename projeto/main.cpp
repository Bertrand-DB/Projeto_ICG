#include <GL/glut.h>
#include <cmath>

// Ângulos para a rotação dos planetas
float earthAngle = 0.0f;
float marsAngle = 0.0f;
//float cameraAngle = 0.0f;

//Struct para ajudar na hora de passar cores
struct Color {
    GLfloat red;
    GLfloat green;
    GLfloat blue;

    Color(GLfloat r, GLfloat g, GLfloat b) : red(r), green(g), blue(b) {}
};

//Cores dos planetas
Color earthColor(0.0f, 0.0f, 1.0f);
Color marsColor(1.0f, 0.0f, 0.0f);

//Função para desenhar plantas!
//distance: distancia da origem | size: tamanho do planete | angle: angulo de translação | Color: obj do tipo Color (R, B, G) 
void drawPlanet(float distance, float size, float angle, Color cor) {
    glPushMatrix();
    glRotatef(angle, 0.0f, 1.0f, 0.0f); // Rotação do planeta ao redor do Sol
    glTranslatef(distance, 0.0f, 0.0f); // Translação em relação ao Sol
    glColor3f(cor.red, cor.green, cor.blue);// Cor do planeta
    glutSolidSphere(size, 50, 50);      // Desenhar o planeta
    glPopMatrix();
}


void display(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //A função define a cor usada para limpar o buffer de cor 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Limpa a janela e repinta com a cor que é passada para a função glClearColor
    
    glLoadIdentity();
     // Definir a posição da câmera
    //         x    y     z
    gluLookAt(0.0, 5.0, 10.0,   // Posição da câmera (olho) no ponto (0, 0, 5)
              0.0, 0.0, 0.0,   // Ponto para o qual a câmera está olhando (0, 0, 0)
              0.0, 1.0, 0.0); 


    // Desenhar o Sol
    glColor3f(1.0f, 1.0f, 0.0f);  // Definimos as cores para o sol
    glutSolidSphere(1.0f, 50, 50); // Desenhar o Sol como uma esfera

    // Desenhar a Terra
    drawPlanet(3.0f, 0.3f, earthAngle, earthColor);

    // Desenhar a Marte
    drawPlanet(5.0f, 0.15f, earthAngle, marsColor);
    
    glutSwapBuffers();  // Troca os buffers de exibição (duplo buffering) para evitar flickering
}
 
// Função chamada quando a janela é redimensionada
void reshape(int w, int h) {
    glViewport(0, 0, w, h);  // Define a área da janela onde o OpenGL pode desenhar (viewport)
    glMatrixMode(GL_PROJECTION);  // Define que vamos alterar a matriz de projeção (para ajustar a câmera)
    glLoadIdentity();  // Carrega a matriz de identidade (reseta a matriz de projeção)
    // Define a perspectiva com um campo de visão de 45 graus, razão de aspecto igual à da janela e clipe entre 1 e 100 unidades
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);  // Volta para a matriz de visualização/modelo, usada para transformar objetos
}

void update(int value) {
    earthAngle += 2.0f;  // Velocidade da rotação da Terra
    if (earthAngle > 360) earthAngle -= 360;

    marsAngle += 1.5f;   // Velocidade da rotação de Marte
    if (marsAngle > 360) marsAngle -= 360;

    glutPostRedisplay();   // Solicitar a atualização da tela
    glutTimerFunc(16, update, 0);  // Chamar esta função a cada ~16ms (~60 FPS)
}


void init() {
    glEnable(GL_DEPTH_TEST);  // Ativar o teste de profundidade
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Fundo preto
}

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //Duplo buffer (GLUT_DOUBLE): Evita tremulação ao desenhar gráficos - Modo de cor RGB (GLUT_RGB): Usa cores vermelho, verde e azul - Buffer de profundidade (GLUT_DEPTH): Armazena informações de profundidade para renderização 3D correta.
    glutInitWindowSize(800, 600); //Tamanho da janela
    glutCreateWindow("Sistema Solar");//Nomeamos a janela aqui
 
    init();//Definições basicas para  o OpenGL
    glutDisplayFunc(display); //Passamos a função que será chamada sempre que a janela necessitar ser redesenhada
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0);
    glutMainLoop(); //A função `glutMainLoop()` entra no loop de eventos do GLUT, mantendo o programa em execução.

    return 1;

}

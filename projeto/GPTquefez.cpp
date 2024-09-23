#include <GL/glut.h>
#include <cmath>

// Ângulos para a rotação dos planetas
float earthAngle = 0.0f;
float marsAngle = 0.0f;
float cameraAngle = 0.0f;

void init() {
    glEnable(GL_DEPTH_TEST);  // Ativar o teste de profundidade
    glEnable(GL_LIGHTING);    // Ativar iluminação
    glEnable(GL_LIGHT0);      // Usar a luz 0
    glEnable(GL_COLOR_MATERIAL);  // Usar cores nos materiais
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Fundo preto
}

void drawPlanet(float distance, float size, float angle) {
    glPushMatrix();
    glRotatef(angle, 0.0f, 1.0f, 0.0f); // Rotação do planeta ao redor do Sol
    glTranslatef(distance, 0.0f, 0.0f); // Translação em relação ao Sol
    glColor3f(0.2f, 0.5f, 1.0f);       // Cor do planeta
    glutSolidSphere(size, 50, 50);      // Desenhar o planeta
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Definir a posição da câmera
    gluLookAt(8.0 * sin(cameraAngle), 2.0, 8.0 * cos(cameraAngle),
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Luz
    GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Desenhar o Sol
    glColor3f(1.0f, 1.0f, 0.0f);  // Amarelo para o Sol
    glutSolidSphere(1.0f, 50, 50); // Desenhar o Sol como uma esfera

    // Desenhar a Terra (distância 3.0, tamanho 0.3, ângulo da órbita)
    drawPlanet(3.0f, 0.3f, earthAngle);

    // Desenhar Marte (distância 5.0, tamanho 0.2, ângulo da órbita)
    drawPlanet(5.0f, 0.2f, marsAngle);

    glutSwapBuffers();
}

void update(int value) {
    earthAngle += 2.0f;  // Velocidade da rotação da Terra
    if (earthAngle > 360) earthAngle -= 360;

    marsAngle += 1.5f;   // Velocidade da rotação de Marte
    if (marsAngle > 360) marsAngle -= 360;

    cameraAngle += 0.01f;  // Mover a câmera lentamente
    if (cameraAngle > 360) cameraAngle -= 360;

    glutPostRedisplay();   // Solicitar a atualização da tela
    glutTimerFunc(16, update, 0);  // Chamar esta função a cada ~16ms (~60 FPS)
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
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}


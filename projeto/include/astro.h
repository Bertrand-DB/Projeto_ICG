#ifndef ASTRO_H
#define ASTRO_H

#include <GL/glut.h> // Inclui a biblioteca GLUT

class Astro {
public:
    float distancia;          // Distância do astro em relação ao Sol
    float raio;               // Raio do astro
    float inclinacaoEixo;     // Inclinação do eixo de rotação
    float anguloRotacao;      // Ângulo de rotação no próprio eixo
    float anguloTranslacao;   // Ângulo de translação (órbita)
    GLuint textura;           // Textura associada ao astro

    Astro(); // Construtor padrão
    ~Astro(); // Destrutor

    Astro(float dist, float r, float incl, float angRot, float angTrans, GLuint tex); // Construtor com parâmetros
};

#endif // ASTRO_H
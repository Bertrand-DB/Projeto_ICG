#ifndef ASTRO_H
#define ASTRO_H

#include <GL/glut.h> // Inclui a biblioteca GLUT

class Astro {
private:
    float distancia;          // Distância do astro em relação ao Sol
    float raio;               // Raio do astro
    float inclinacaoEixo;     // Inclinação do eixo de rotação
    float anguloRotacao;      // Ângulo de rotação no próprio eixo
    float anguloTranslacao;   // Ângulo de translação (órbita)
    GLuint textura;           // Textura associada ao astro

public:
    Astro(); // Construtor padrão
    ~Astro(); // Destrutor

    Astro(float dist, float r, float incl, float angRot, float angTrans, GLuint tex); // Construtor com parâmetros

    float get_distancia();
    float get_raio();
    float get_inclinacaoEixo();
    float get_anguloRotacao();
    float get_anguloTranslacao();
    GLuint get_textura();

    void set_distancia(float distancia);
    void set_raio(float raio);
    void set_inclinacaoEixo(float inclinacaoEixo);
    void set_anguloRotacao(float anguloRotacao);
    void set_anguloTranslacao(float anguloTranslacao);
    void set_textura(GLuint textura);

    void draw();

    void update_anguloRotacao(float anguloRotacao);
    void update_anguloTranslacao(float anguloTranslacao);
};

#endif // ASTRO_H
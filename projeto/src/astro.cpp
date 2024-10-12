#include "../include/astro.h"

Astro::Astro() {}
Astro::~Astro() {}

Astro::Astro(float dist, float r, float incl, float angRot, float angTrans, GLuint tex)
    : distancia(dist), raio(r), inclinacaoEixo(incl), anguloRotacao(angRot), anguloTranslacao(angTrans), textura(tex) {}
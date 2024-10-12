#include "../include/astro.h"

Astro::Astro() {}
Astro::~Astro() {}

Astro::Astro(float dist, float r, float incl, float angRot, float angTrans, GLuint tex)
    : distancia(dist), raio(r), inclinacaoEixo(incl), anguloRotacao(angRot), anguloTranslacao(angTrans), textura(tex) {}

float Astro::get_distancia()
{
    return this->distancia;
}

float Astro::get_raio()
{
    return this->raio;
}

float Astro::get_inclinacaoEixo()
{
    return this->inclinacaoEixo;
}

float Astro::get_anguloRotacao()
{
    return this->anguloRotacao;
}

float Astro::get_anguloTranslacao()
{
    return this->anguloTranslacao;
}

GLuint Astro::get_textura()
{
    return this->textura;
}

void Astro::set_distancia(float distancia)
{
    this->distancia = distancia;
}

void Astro::set_raio(float raio)
{
    this->raio = raio;
}

void Astro::set_inclinacaoEixo(float inclinacaoEixo)
{
    this->inclinacaoEixo = inclinacaoEixo;
}

void Astro::set_anguloRotacao(float anguloRotacao)
{
    this->anguloRotacao = anguloRotacao;
}

void Astro::set_anguloTranslacao(float anguloTranslacao)
{
    this->anguloTranslacao = anguloTranslacao;
}

void Astro::set_textura(GLuint textura)
{
    this->textura = textura;
}

void Astro::update_anguloRotacao(float angulo)
{
    this->anguloRotacao += angulo;
}

void Astro::update_anguloTranslacao(float angulo)
{
    this->anguloTranslacao += angulo;
}   

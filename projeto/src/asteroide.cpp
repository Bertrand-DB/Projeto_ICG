#include <vector>
#include <math.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

using glm::perlin;
using glm::vec2;
using glm::vec3;

struct VetorCartesiano {
    float x, y, z;
};



class Asteroide
{
private:
    std::vector<VetorCartesiano> vertices;
    int segmentos_latitude;
    int segmentos_longitude;
    int raio;
    int distancia;
    float magnitude;  //Intensidade das deformações do ruído
    GLuint textura;
    
    float perlin_acumulativo(float x, float y, float z, int n_amostras, float atenuacao);
    void set_vertices();

public:
    Asteroide(int raio, int distancia, float magnitude, int seg_lat, int seg_long);
    ~Asteroide();

    void draw();
    void flat_draw();
    void set_textura(const char* texture_path);
    GLuint get_textura();
};

Asteroide::Asteroide(int raio, int distancia, float magnitude=0.6, int seg_lat=60, int seg_long=60)
{
    this->raio = raio;
    this->segmentos_latitude = seg_lat;
    this->segmentos_longitude = seg_long;
    this->distancia = distancia;
    this->magnitude = magnitude;
    this->textura = 0;
    
    set_vertices();
}

Asteroide::~Asteroide()
{
}

float Asteroide::perlin_acumulativo(float x, float y, float z, int n_amostras, float atenuacao)
{
    float valor = 0;
    float frequencia = 1;
    float amplitude = 1;

    for (size_t i = 0; i < n_amostras; i++)
    {
        vec3 vetor(x * frequencia*atenuacao, y * frequencia*atenuacao, z *frequencia*atenuacao);
        valor += perlin(vetor) * amplitude;

        frequencia *= 2;
        amplitude /= 2;
    }
    
    return valor;
}

//Calcula e armazena os vértices necessários usando ângulos de latitude e longitude
void Asteroide::set_vertices()
{
    // Loop através dos segmentos de latitude para criar os "anéis" horizontais da esfera
    for (int lat = 0; lat <= segmentos_latitude; ++lat) {
        float theta = lat * M_PI / segmentos_latitude;  // Calcula o ângulo da latitude atual (de 0 a PI)

        // Loop através dos segmentos de longitude para criar os pontos ao longo de cada "anel" de latitude
        for (int lon = 0; lon <= segmentos_longitude; ++lon) {
            float phi = lon * 2.0f * M_PI / segmentos_longitude;    // Calcula o ângulo de longitude atual (de 0 a 2*PI)

            VetorCartesiano vertice;    // Vértice para a posição atual da esfera
            vertice.x = cos(phi) * sin(theta);   // Coordenada X do vértice
            vertice.y = cos(theta);              // Coordenada Y do vértice
            vertice.z = sin(phi) * sin(theta);   // Coordenada Z do vértice
            
            //const vec3 vect_esferico(vertice.x, vertice.y, vertice.z);
            float ruido = perlin_acumulativo(vertice.x, vertice.y, vertice.z, 4, 0.4);//Calcula o ruído proporcional ao raio do asteroide
            float raio_perturbado = raio + magnitude*raio*ruido;

            vertice.y *= raio_perturbado;
            vertice.x *= raio_perturbado;
            vertice.z *= raio_perturbado;
            
            // Adiciona o vértice calculado ao vetor de vértices da esfera
            this->vertices.push_back(vertice);
        }
    }
}

//Carrega e configura a textura a partir de um caminho de arquivo. Usar apenas em um contexto OpenGL ativo!
void Asteroide::set_textura(const char* texture_path)
{
    GLuint tex = SOIL_load_OGL_texture(texture_path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

    if (!tex) {
        std::cerr << "Falha ao carregar a textura: " << texture_path << std::endl;
        exit(1); // Encerra o programa em caso de falha no carregamento
    }
    this->textura = tex;    
    
    glBindTexture(GL_TEXTURE_2D, textura); // Vincula a textura atual

    // Configurações de repetição de textura (wrap)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Envolve no eixo S
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Envolve no eixo T

    // Configurações de filtros de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtro para minificação
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtro para ampliação
}

GLuint Asteroide::get_textura(){
    return this->textura;
}

void Asteroide::draw() {
    glPushMatrix(); // Preserva o estado atual da matriz

    glTranslatef(distancia, 0.0f, 0.0f);

    // Habilita a textura
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textura);
    
    // Base branca para não alterar a cor da textura
    glColor3f(1.0f, 1.0f, 1.0f); 

    // Renderiza a esfera com textura
    glBegin(GL_TRIANGLE_STRIP);
    if (textura)
    {
        for (int lat = 0; lat < segmentos_latitude; ++lat) {
            for (int lon = 0; lon <= segmentos_longitude; ++lon) {

                int current = lat * (segmentos_longitude + 1) + lon;    // Indice dos vértices superiores
                glTexCoord2f((float)lon / segmentos_longitude, (float)lat / segmentos_latitude);                // vincula a textura a cada vértice
                glNormal3f(vertices[current].x / raio, vertices[current].y / raio, vertices[current].z / raio); // normaliza os vetores de cada vértice para garantir a iluminação correta
                glVertex3f(vertices[current].x, vertices[current].y, vertices[current].z);                      // define os vertices superiores dos triângulos

                int next = current + segmentos_longitude + 1;   // Indice dos vértices inferiores
                glTexCoord2f((float)lon / segmentos_longitude, (float)(lat + 1) / segmentos_latitude);  // vincula a textura a cada vértice
                glNormal3f(vertices[next].x / raio, vertices[next].y / raio, vertices[next].z / raio);  // normaliza os vetores de cada vértice para garantir a iluminação correta
                glVertex3f(vertices[next].x, vertices[next].y, vertices[next].z);                       // define os vertices inferiores dos triângulos
            }
        }
    }
    else
    {
        for (int lat = 0; lat < segmentos_latitude; ++lat) {
            for (int lon = 0; lon <= segmentos_longitude; ++lon) {

                int current = lat * (segmentos_longitude + 1) + lon;    // Indice dos vértices superiores
                glNormal3f(vertices[current].x / raio, vertices[current].y / raio, vertices[current].z / raio); // normaliza os vetores de cada vértice para garantir a iluminação correta
                glVertex3f(vertices[current].x, vertices[current].y, vertices[current].z);                      // define os vertices superiores dos triângulos

                int next = current + segmentos_longitude + 1;   // Indice dos vértices inferiores
                glNormal3f(vertices[next].x / raio, vertices[next].y / raio, vertices[next].z / raio);  // normaliza os vetores de cada vértice para garantir a iluminação correta
                glVertex3f(vertices[next].x, vertices[next].y, vertices[next].z);                       // define os vertices inferiores dos triângulos
            }
        }
    }
    
    glEnd();

    // Desabilita a textura
    glDisable(GL_TEXTURE_2D);

    glPopMatrix(); // Restaura o estado da matriz
}

void Asteroide::flat_draw()
{
    for (size_t x = 0; x < segmentos_latitude; x++)
    {
        /* code */
    }
    
}
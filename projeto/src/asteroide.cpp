#include <vector>
#include <math.h>
#include <GL/glut.h>

struct Vertice {
    float x, y, z;
};

class Asteroide
{
private:
    std::vector<Vertice> vertices;
    int segmentos_latitude;
    int segmentos_longitude;
    int raio;
    int distancia;
    GLuint textura;
    
    void set_vertices();

public:
    Asteroide(int raio, int seg_lat, int seg_long, int distancia, GLuint textura);
    ~Asteroide();

    void draw();
    void set_textura(GLuint textura);
    GLuint get_textura();
};

Asteroide::Asteroide(int raio, int seg_lat, int seg_long, int distancia, GLuint textura=0){
    this->raio = raio;
    this->segmentos_latitude = seg_lat;
    this->segmentos_longitude = seg_long;
    this->distancia = distancia;
    this->textura = textura;

    set_vertices();
}

Asteroide::~Asteroide()
{
}

void Asteroide::set_vertices(){
    for (int lat = 0; lat <= segmentos_latitude; ++lat) {
        float theta = lat * M_PI / segmentos_latitude;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= segmentos_longitude; ++lon) {
            float phi = lon * 2.0f * M_PI / segmentos_longitude;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            Vertice vertice;
            vertice.x = raio * cosPhi * sinTheta;
            vertice.y = raio * cosTheta;
            vertice.z = raio * sinPhi * sinTheta;

            this->vertices.push_back(vertice);
        }
    }
}

void Asteroide::set_textura(GLuint textura){
    this->textura = textura;
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
    glEnd();

    // Desabilita a textura
    glDisable(GL_TEXTURE_2D);

    glPopMatrix(); // Restaura o estado da matriz
}

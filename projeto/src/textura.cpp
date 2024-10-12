#include "../include/textura.h"

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

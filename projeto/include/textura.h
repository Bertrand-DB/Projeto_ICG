#ifndef TEXTURA_H
#define TEXTURA_H

#include <iostream>
#include <GL/glut.h>
#include <SOIL/SOIL.h>

GLuint loadTexture(const char* path);
void configurarTextura(GLuint texturaID);

#endif
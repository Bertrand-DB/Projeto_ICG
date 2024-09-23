cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(OpenGLProject)

# Definir a versão do C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Adicionar os arquivos de origem
add_executable(main main.cpp)

# Encontrar pacotes necessários
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Incluir diretórios
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

# Ligar bibliotecas
target_link_libraries(main ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)

#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <GL/glu.h>

class Camera {
private:
    float posX, posY, posZ;  // Posição da câmera
    float lookX, lookY, lookZ; // Direção para onde a câmera está olhando
    float angleH, angleV;  // Ângulos da câmera
    float movementSpeed, rotationSpeed;  // Velocidade de movimento e rotação

public:
    Camera();
    ~Camera();

    Camera(float x, float y, float z, float lookX, float lookY, float lookZ, float angH, float angV, float moveSpeed, float rotSpeed);

    void applyView();
    
    // Métodos para mover e rotacionar a câmera
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void rotateUp();
    void rotateDown();
    void updateLookDirection();
    void update_lookUp();
    void update_lookDown();
    void update_lookLeft();
    void update_lookRight();

    float get_posX();
    float get_posY();
    float get_posZ();
    float get_lookX();
    float get_lookY();
    float get_lookZ();
    float get_angleH();
    float get_angleV();
    float get_movementSpeed();
    float get_rotationSpeed();

    void set_posX(float posX);
    void set_posY(float posY);
    void set_posZ(float posZ);
    void set_lookX(float lookX);
    void set_lookY(float lookY);
    void set_lookZ(float lookZ);
    void set_angleH(float angleH);
    void set_angleV(float angleV);
    void set_movementSpeed(float movementSpeed);
    void set_rotationSpeed(float rotationSpeed);
};

#endif // CAMERA_H
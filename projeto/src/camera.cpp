#include "../include/camera.h"

Camera::Camera() {}
Camera::~Camera() {}

Camera::Camera(float x, float y, float z, float lookX, float lookY, float lookZ, float angH, float angV, float moveSpeed, float rotSpeed)
    : posX(x), posY(y), posZ(z), lookX(lookX), lookY(lookY), lookZ(lookZ), angleH(angH), angleV(angV), movementSpeed(moveSpeed), rotationSpeed(rotSpeed) {}

void Camera::applyView() {
    gluLookAt(this->posX, this->posY, this->posZ,
              this->posX + this->lookX, this->posY + this->lookY, this->posZ + this->lookZ,
              0.0, 1.0, 0.0);
}

void Camera::moveForward()
{
    this->posX += this->lookX * this->movementSpeed;
    this->posZ += this->lookZ * this->movementSpeed;
}

void Camera::moveBackward()
{
    this->posX -= this->lookX * this->movementSpeed;
    this->posZ -= this->lookZ * this->movementSpeed;
}

void Camera::moveLeft()
{
    this->posX += this->lookZ * this->movementSpeed;
    this->posZ -= this->lookX * this->movementSpeed;
}

void Camera::moveRight()
{
    this->posX -= this->lookZ * this->movementSpeed;
    this->posZ += this->lookX * this->movementSpeed;
}

void Camera::moveUp()
{
    this->posY += this->movementSpeed;
}

void Camera::moveDown()
{
    this->posY -= this->movementSpeed;
}

void Camera::rotateUp()
{
    this->lookY += this->movementSpeed;
}

void Camera::rotateDown()
{
    this->lookY -= this->movementSpeed;
}

void Camera::updateLookDirection()
{
    this->lookX = sin(this->angleH) * cos(this->angleV);
    this->lookY = sin(this->angleV);
    this->lookZ = -cos(this->angleH) * cos(this->angleV);
}

void Camera::update_lookUp()
{
    this->angleV += this->rotationSpeed;
}

void Camera::update_lookDown()
{
    this->angleV -= this->rotationSpeed;
}

void Camera::update_lookLeft()
{
    this->angleH -= this->rotationSpeed;
}

void Camera::update_lookRight()
{
    this->angleH += this->rotationSpeed;
}

float Camera::get_posX()
{
    return this->posX;
}

float Camera::get_posY()
{
    return this->posY;
}

float Camera::get_posZ()
{
    return this->posZ;
}

float Camera::get_lookX()
{
    return this->lookX;
}

float Camera::get_lookY()
{
    return this->lookY;
}

float Camera::get_lookZ()
{
    return this->lookZ;
}

float Camera::get_angleH()
{
    return this->angleH;
}

float Camera::get_angleV()
{
    return this->angleV;
}

float Camera::get_movementSpeed()
{
    return this->movementSpeed;
}

float Camera::get_rotationSpeed()
{
    return this->rotationSpeed;
}

void Camera::set_posX(float posX)
{
    this->posX = posX;
}

void Camera::set_posY(float posY)
{
    this->posY = posY;
}

void Camera::set_posZ(float posZ)
{
    this->posZ = posZ;
}

void Camera::set_lookX(float lookX)
{
    this->lookX = lookX;
}

void Camera::set_lookY(float lookY)
{
    this->lookY = lookY;
}

void Camera::set_lookZ(float lookZ)
{
    this->lookZ = lookZ;
}

void Camera::set_angleH(float angleH)
{
    this->angleH = angleH;
}

void Camera::set_angleV(float angleV)
{   
    if (angleV < -1.5) this->angleV = -1.5;     //garantir os limites de ângulação vertical
    else if (angleV > 1.5)this->angleV = 1.5;
    else this->angleV = angleV;
}

void Camera::set_movementSpeed(float movementSpeed)
{
    this->movementSpeed = movementSpeed;
}

void Camera::set_rotationSpeed(float rotationSpeed)
{
    this->rotationSpeed = rotationSpeed;
}

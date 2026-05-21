#include "game_object.hpp"

GameObject::~GameObject() {}

//================================================================================== 
// Global Movement
void GameObject::moveGlobalX(float deltaX) { this->transform.m12 += deltaX; }
void GameObject::moveGlobalY(float deltaY) { this->transform.m13 += deltaY; }
void GameObject::moveGlobalZ(float deltaZ) { this->transform.m14 += deltaZ; }

void GameObject::moveGlobal(float deltaX, float deltaY, float deltaZ) {
    moveGlobalX(deltaX); moveGlobalY(deltaY); moveGlobalZ(deltaZ); 
}

//================================================================================== 
// Local Movement
Vector3 GameObject::getPosition() const { return (Vector3){ transform.m12, transform.m13, transform.m14 }; }
Vector3 GameObject::getForward() const { return (Vector3){ -transform.m8, -transform.m9, -transform.m10 }; }
Vector3 GameObject::getUp()      const { return (Vector3){ transform.m4,  transform.m5,  transform.m6 }; }
Vector3 GameObject::getRight()   const { return (Vector3){ transform.m0,  transform.m1,  transform.m2 }; }

void GameObject::moveLocalForward(float distance) {
    Matrix moveMat = MatrixTranslate(0.0f, 0.0f, -distance);
    this->transform = MatrixMultiply(moveMat, this->transform);
}

void GameObject::moveLocalRight(float distance) {
    Matrix moveMat = MatrixTranslate(distance, 0.0f, 0.0f);
    this->transform = MatrixMultiply(moveMat, this->transform);
}

void GameObject::moveLocalUp(float distance) {
    Matrix moveMat = MatrixTranslate(0.0f, distance, 0.0f);
    this->transform = MatrixMultiply(moveMat, this->transform);
}

//================================================================================== 
// Rotation
void GameObject::rotateYaw(float angleRad) {
    this->transform = MatrixMultiply(MatrixRotateY(angleRad), this->transform);
}
void GameObject::rotatePitch(float angleRad) {
    this->transform = MatrixMultiply(MatrixRotateX(angleRad), this->transform);
}
void GameObject::rotateRoll(float angleRad) {
    this->transform = MatrixMultiply(MatrixRotateZ(angleRad), this->transform);
}

void GameObject::rotate(float deltaPitch, float deltaYaw, float deltaRoll) {
    Matrix pMat = MatrixRotateX(deltaPitch);
    Matrix yMat = MatrixRotateY(deltaYaw);
    Matrix rMat = MatrixRotateZ(deltaRoll);

    Matrix frameRotation = MatrixMultiply(MatrixMultiply(pMat, yMat), rMat);

    this->transform = MatrixMultiply(frameRotation, this->transform);
}

#include "game_object.hpp"
#include "raymath.h"

//================================================================================== 
// Global Movement

void GameObject::setPositionX(float x) {
    this->transform.m12 = x; 
}
void GameObject::setPositionY(float y) {
    this->transform.m13 = y; 
}
void GameObject::setPositionZ(float z) {
    this->transform.m14 = z; 
}
void GameObject::setPosition(float x, float y, float z) {
    setPositionX(x); 
    setPositionY(y); 
    setPositionZ(z);
}
void GameObject::setPosition(Vector3 position) {
    setPositionX(position.x); 
    setPositionY(position.y); 
    setPositionZ(position.z);
}
void GameObject::moveGlobalX(float deltaX) { this->transform.m12 += deltaX; }
void GameObject::moveGlobalY(float deltaY) { this->transform.m13 += deltaY; }
void GameObject::moveGlobalZ(float deltaZ) { this->transform.m14 += deltaZ; }

void GameObject::moveGlobal(float deltaX, float deltaY, float deltaZ) {
    moveGlobalX(deltaX); moveGlobalY(deltaY); moveGlobalZ(deltaZ); 
}

void GameObject::moveGlobal(Vector3 delta) {
    moveGlobalX(delta.x); moveGlobalY(delta.y); moveGlobalZ(delta.z); 
}

void GameObject::moveGlobalVel(Vector3 velocity) {
    Vector3 displacement = Vector3Scale(velocity, GetFrameTime());
    moveGlobal(displacement);
}

//================================================================================== 
// Local Movement

Vector3 GameObject::getPosition() const { 
    return (Vector3){ transform.m12, transform.m13, transform.m14 }; 
}

Vector3 GameObject::getRight() const { 
    return (Vector3){ transform.m0,  transform.m1,  transform.m2 }; 
}
Vector3 GameObject::getUp() const { 
    return (Vector3){ transform.m4,  transform.m5,  transform.m6 }; 
}
Vector3 GameObject::getForward() const { 
    return (Vector3){ -transform.m8, -transform.m9, -transform.m10 }; 
}

void GameObject::moveLocalRight(float distance) {
    Matrix moveMat = MatrixTranslate(distance, 0.0f, 0.0f);
    this->transform = moveMat * this->transform;
}
void GameObject::moveLocalUp(float distance) {
    Matrix moveMat = MatrixTranslate(0.0f, distance, 0.0f);
    this->transform = moveMat * this->transform;
}
void GameObject::moveLocalForward(float distance) {
    Matrix moveMat = MatrixTranslate(0.0f, 0.0f, -distance);
    this->transform = moveMat * this->transform;
}

//================================================================================== 
// Rotation
void GameObject::rotatePitch(float angleRad) {
    this->transform = MatrixRotateX(angleRad) * this->transform;
}
void GameObject::rotateYaw(float angleRad) {
    this->transform = MatrixRotateY(angleRad) * this->transform;
}
void GameObject::rotateRoll(float angleRad) {
    this->transform = MatrixRotateZ(angleRad) * this->transform;
}

void GameObject::rotate(float deltaPitch, float deltaYaw, float deltaRoll) {
    Matrix pMat = MatrixRotateX(deltaPitch);
    Matrix yMat = MatrixRotateY(deltaYaw);
    Matrix rMat = MatrixRotateZ(deltaRoll);

    Matrix frameRotation = pMat * yMat * rMat;

    this->transform = frameRotation * this->transform;
}

//================================================================================== 
// Frame by frame behavior
void GameObject::onUpdate(Scene& scene){
    update(scene);
}

void GameObject::onBeforeUpdate(Scene& scene){
    beforeUpdate(scene);
}

void GameObject::onAfterUpdate(Scene& scene){
    afterUpdate(scene);
}

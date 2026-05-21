#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "raylib.h"
#include "raymath.h"

class GameObject {
public:
    virtual ~GameObject();

    // Master matrix tracking position and orientation
    Matrix transform = MatrixIdentity();

    Model *model = nullptr;
    Matrix modelBaseRotation = MatrixIdentity();

    // Getters
    Vector3 getPosition() const;
    Vector3 getForward() const;
    Vector3 getUp() const;
    Vector3 getRight() const;

    // Setters
    void setPositionX(float x);
    void setPositionY(float y);
    void setPositionZ(float z);
    void setPosition(float x, float y, float z);
    void setPosition(Vector3 position);

    // Global Movement
    virtual void moveGlobalX(float deltaX);
    virtual void moveGlobalY(float deltaY);
    virtual void moveGlobalZ(float deltaZ);
    virtual void moveGlobal(float deltaX, float deltaY, float deltaZ);
    virtual void moveGlobal(Vector3 delta);

    // Local Movement
    virtual void moveLocalRight(float distance);
    virtual void moveLocalUp(float distance);
    virtual void moveLocalForward(float distance);

    // Rotations
    virtual void rotatePitch(float angleRad); // Rotation around Right axis
    virtual void rotateYaw(float angleRad); // Rotation around Up axis
    virtual void rotateRoll(float angleRad); // Rotation around Forward axis
    virtual void rotate(float deltaPitch, float deltaYaw, float deltaRoll);

    // Frame by frame behavior
    virtual void onUpdate();
protected:
    virtual void update() {};
};

#endif // GAMEOBJECT_HPP

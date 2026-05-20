#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "raylib.h"
#include "raymath.h"

class GameObject {
public:
    virtual ~GameObject();

    // Master matrix tracking position and orientation
    Matrix transform = MatrixIdentity();

    // Getters
    Vector3 getPosition() const;
    Vector3 getForward() const;
    Vector3 getUp() const;
    Vector3 getRight() const;

    // Global Movement
    virtual void moveGlobalX(float deltaX);
    virtual void moveGlobalY(float deltaY);
    virtual void moveGlobalZ(float deltaZ);
    virtual void moveGlobal(float deltaX, float deltaY, float deltaZ);

    // Local Movement
    virtual void moveLocalForward(float distance);
    virtual void moveLocalRight(float distance);
    virtual void moveLocalUp(float distance);

    // Rotations
    virtual void rotateYaw(float angleRad);
    virtual void rotatePitch(float angleRad);
    virtual void rotateRoll(float angleRad);
    virtual void rotate(float deltaPitch, float deltaYaw, float deltaRoll);
};

#endif // GAMEOBJECT_HPP

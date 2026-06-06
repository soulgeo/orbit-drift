#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "raylib.h"
#include "raymath.h"

class Scene;

class GameObject {
public:
    virtual ~GameObject() {};

    // Master matrix tracking position and orientation
    Matrix transform = MatrixIdentity();

    // Getters
    // TODO: Add Quaternion Getter
    Vector3 getPosition() const;
    Vector3 getForward() const;
    Vector3 getUp() const;
    Vector3 getRight() const;

    // Setters
    // TODO: Add Quaternion Setter
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
    virtual void moveGlobalVel(Vector3 velocity);

    // Local Movement
    virtual void moveLocalRight(float distance);
    virtual void moveLocalUp(float distance);
    virtual void moveLocalForward(float distance);

    // Rotations
    // TODO: Add Quaternion Rotation
    virtual void rotatePitch(float angleRad); // Rotation around Right axis
    virtual void rotateYaw(float angleRad); // Rotation around Up axis
    virtual void rotateRoll(float angleRad); // Rotation around Forward axis
    virtual void rotate(float deltaPitch, float deltaYaw, float deltaRoll);

    // Frame by frame behavior
    virtual void onUpdate(Scene& scene);
    virtual void onBeforeUpdate(Scene& scene);
    virtual void onAfterUpdate(Scene& scene);

protected:
    virtual void update(Scene& scene) {};
    virtual void beforeUpdate(Scene& scene) {};
    virtual void afterUpdate(Scene& scene) {};
};

#endif // GAMEOBJECT_HPP

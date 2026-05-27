#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include "game_object.hpp"
#include "game.hpp"
#include "raylib.h"

class PlayerShip : public GameObject {
public:
    PlayerShip();
    ~PlayerShip();

    float forwardSpeed = 0.8f;
    float forwardAccel = 1.5f;

    float rightSpeed = 0.4f;
    float rightAccel = 3.0f;

    float upSpeed = 0.4f;
    float upAccel = 3.0f;

    float panSpeed = 0.004f;
    float rollSpeed = 1.0f;

    BoundingBox hitbox;

    bool isInGravitySOI = false;

    void addGravity(Vector3 gravityAccel);

    void update(Game& game) override;

private:
    Vector3 currentVelocity = Vector3Zero();
    Vector3 externalGravityVelocity = Vector3Zero();
};

class Planet : public GameObject {
public:
    Planet(Color color, Vector3 position, float radius, float gravityRadius, float GravityForce);
    ~Planet();

    Color color;

    float radius;
    float gravityRadius;
    float gravityForce;
    float rotationSpeed;

    void update(Game& game) override;
};

#endif // !ENTITIES_HPP

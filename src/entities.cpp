#include "entities.hpp"

#include "scene.hpp"
#include "raylib.h"
#include "raymath.h"

PlayerShip::PlayerShip() {
    hitbox = (BoundingBox){(Vector3){-0.2f, -0.2f, -0.2f}, (Vector3){0.2f, 0.2f, 0.2f}};
}

PlayerShip::~PlayerShip() {}

void PlayerShip::update(Scene& scene) {
    Vector2 mousePosition = GetMousePosition();
    Vector2 mouseDistance = {mousePosition.x - 960, mousePosition.y - 540};
    float localYaw = mouseDistance.x * -panSpeed * GetFrameTime();
    float localPitch = mouseDistance.y * -panSpeed * GetFrameTime();

    rotatePitch(localPitch);
    rotateYaw(localYaw);

    int moveX =
        scene.isActiveInput(MOVE_RIGHT) - scene.isActiveInput(MOVE_LEFT);
    int moveY = scene.isActiveInput(MOVE_UP) - scene.isActiveInput(MOVE_DOWN);
    int moveZ =
        scene.isActiveInput(MOVE_FORWARD) - scene.isActiveInput(MOVE_BACK);

    Vector3 localTargetVelocity = {moveX * rightSpeed, moveY * upSpeed, -moveZ * forwardSpeed};
    currentVelocity = Vector3Lerp(currentVelocity, localTargetVelocity, forwardAccel * GetFrameTime());
    Quaternion rot = QuaternionFromMatrix(transform);

    Vector3 worldMovement = Vector3RotateByQuaternion(currentVelocity, rot);
    worldMovement = Vector3Add(worldMovement, Vector3Scale(externalGravityVelocity, GetFrameTime()));

    moveGlobal(worldMovement);

    externalGravityVelocity = Vector3Zero();

    // Update hitbox position
    Vector3 pos = getPosition();
    hitbox.min = (Vector3){pos.x - 0.2f, pos.y - 0.2f, pos.z - 0.2f};
    hitbox.max = (Vector3){pos.x + 0.2f, pos.y + 0.2f, pos.z + 0.2f};
}

void PlayerShip::addGravity(Vector3 gravityAccel) {
    externalGravityVelocity = Vector3Add(externalGravityVelocity, gravityAccel);
}

Planet::Planet(Vector3 p_position, float p_radius, float p_gravityRadius, float p_gravityForce)
    : radius(p_radius),
      gravityRadius(p_gravityRadius),
      gravityForce(p_gravityForce) 
{
    setPosition(p_position);
}

Planet::~Planet() {}

void Planet::update(Scene& scene) {
    PlayerShip* playerShip = (PlayerShip*)scene.getGameObject("player");
    bool colliding = CheckCollisionBoxSphere(playerShip->hitbox, getPosition(), gravityRadius);
    if (colliding) {
        playerShip->isInGravitySOI = true;
        Vector3 direction = Vector3Normalize(Vector3Subtract(getPosition(), playerShip->getPosition()));
        float distance = Vector3Distance(playerShip->getPosition(), getPosition());
        float distanceFactor = 1.0f - Clamp(distance / gravityRadius, 0, 1);
        Vector3 gravityAccel = Vector3Scale(direction, gravityForce * distanceFactor);

        playerShip->addGravity(gravityAccel);
    }
}

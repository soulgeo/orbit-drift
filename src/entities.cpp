#include "entities.hpp"

#include "raylib.h"
#include "raymath.h"

PlayerShip::PlayerShip() {
    model = LoadModelFromMesh(GenMeshCone(0.2f, 0.5f, 16));
    modelBaseRotation = MatrixRotateX(-90.0f * DEG2RAD);
    hitbox = (BoundingBox){(Vector3){-0.2f, -0.2f, -0.2f}, (Vector3){0.2f, 0.2f, 0.2f}};
}

PlayerShip::~PlayerShip() { UnloadModel(model); }

void PlayerShip::update(Game& game) {
    Vector2 mousePosition = GetMousePosition();
    Vector2 mouseDistance = {mousePosition.x - 960, mousePosition.y - 540};
    float localYaw = mouseDistance.x * -panSpeed * GetFrameTime();
    float localPitch = mouseDistance.y * -panSpeed * GetFrameTime();

    rotatePitch(localPitch);
    rotateYaw(localYaw);

    int moveX =
        game.isActiveInput("move_right") - game.isActiveInput("move_left");
    int moveY = game.isActiveInput("move_up") - game.isActiveInput("move_down");
    int moveZ =
        game.isActiveInput("move_forward") - game.isActiveInput("move_back");

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

Planet::Planet(Color p_color, Vector3 p_position, float p_radius, float p_gravityRadius, float p_gravityForce)
    : color(p_color),
      radius(p_radius),
      gravityRadius(p_gravityRadius),
      gravityForce(p_gravityForce) 
{
    setPosition(p_position);
    model = LoadModelFromMesh(GenMeshSphere(p_radius, 30, 30));
}

Planet::~Planet() { UnloadModel(model); }

void Planet::update(Game& game) {
    PlayerShip* playerShip = (PlayerShip*)game.getGameObject("player");
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

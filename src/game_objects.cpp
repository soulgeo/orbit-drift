#include "game_objects.hpp"

#include "raylib.h"

PlayerShip::PlayerShip() {
    // Load Player Model
    model = LoadModelFromMesh(GenMeshCone(0.2f, 0.5f, 16));
    modelBaseRotation = MatrixRotateX(-90.0f * DEG2RAD);
}

PlayerShip::~PlayerShip() {
    // Unload Player Model
    UnloadModel(model);
}

void PlayerShip::update() {
    Vector2 mousePosition = GetMousePosition();
    Vector2 mouseDistance = {mousePosition.x - 960, mousePosition.y - 540};
    float localYaw = mouseDistance.x * -panSpeed * GetFrameTime();
    float localPitch = mouseDistance.y * -panSpeed * GetFrameTime();

    rotatePitch(localPitch);
    rotateYaw(localYaw);
}

void PlayerShip::handleAction(const std::string& action) {
    if (action == "move_up") {
        moveLocalUp(forwardSpeed * GetFrameTime());
    }
    if (action == "move_down") {
        moveLocalUp(-forwardSpeed * GetFrameTime());
    }
    if (action == "move_forward") {
        moveLocalForward(forwardSpeed * GetFrameTime());
    }
    if (action == "move_back") {
        moveLocalForward(-forwardSpeed * GetFrameTime());
    }
    if (action == "move_left") {
        moveLocalRight(-forwardSpeed * GetFrameTime());
    }
    if (action == "move_right") {
        moveLocalRight(forwardSpeed * GetFrameTime());
    }
    if (action == "roll_cw") {
        rotateRoll(-rollSpeed * GetFrameTime());
    }
    if (action == "roll_ccw") {
        rotateRoll(rollSpeed * GetFrameTime());
    }
}

Planet::Planet(float p_radius, Vector3 p_position) : radius(p_radius) {
    setPosition(p_position);
    model = LoadModelFromMesh(GenMeshSphere(p_radius, 30, 30));
}

Planet::~Planet() { UnloadModel(model); }

void Planet::update() {
    // Nothing for now
}

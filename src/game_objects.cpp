#include "game_objects.hpp"
#include "raylib.h"

PlayerShip::PlayerShip() {
    // Load Player Model
    Mesh coneMesh = GenMeshCone(0.2f, 0.5f, 16);
    Model playerModel = LoadModelFromMesh(coneMesh);
    Matrix baseRotation = 
    modelBaseRotation = MatrixRotateX(-90.0f * DEG2RAD);;
    model = playerModel;
}

PlayerShip::~PlayerShip() {
    // Unload Player Model
    UnloadModel(model);
}

void PlayerShip::update() {
    Vector2 mousePosition = GetMousePosition();
    Vector2 mouseDistance = { 
        mousePosition.x - 960,
        mousePosition.y - 540 
    };
    float localYaw   = mouseDistance.x * -panSpeed * GetFrameTime();
    float localPitch = mouseDistance.y * -panSpeed * GetFrameTime();

    rotatePitch(localPitch);
    rotateYaw(localYaw);
}

void PlayerShip::handleAction(const std::string& action){
    if (action == "move_up") {moveLocalUp(forwardSpeed*GetFrameTime());}
    if (action == "move_down") {moveLocalUp(-forwardSpeed*GetFrameTime());}
    if (action == "move_forward") {moveLocalForward(forwardSpeed*GetFrameTime());}
    if (action == "move_back") {moveLocalForward(-forwardSpeed*GetFrameTime());}
    if (action == "move_left") {moveLocalRight(-forwardSpeed*GetFrameTime());}
    if (action == "move_right") {moveLocalRight(forwardSpeed*GetFrameTime());}
    if (action == "roll_cw") {rotateRoll(-rollSpeed*GetFrameTime());}
    if (action == "roll_ccw") {rotateRoll(rollSpeed*GetFrameTime());}
}

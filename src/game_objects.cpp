#include "game_objects.hpp"
#include "raylib.h"

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
    if (action == "move_forward") {moveLocalForward(forwardSpeed*GetFrameTime());}
    if (action == "move_back") {moveLocalForward(-forwardSpeed*GetFrameTime());}
    if (action == "move_left") {moveLocalRight(-forwardSpeed*GetFrameTime());}
    if (action == "move_right") {moveLocalRight(forwardSpeed*GetFrameTime());}
    if (action == "roll_cw") {rotateRoll(-rollSpeed*GetFrameTime());}
    if (action == "roll_ccw") {rotateRoll(rollSpeed*GetFrameTime());}
}

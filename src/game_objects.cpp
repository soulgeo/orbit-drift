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

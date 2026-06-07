#include "engine.hpp"
#include "raylib.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbit Drift");
    SetTargetFPS(60);

    Engine engine;
    engine.run();

    CloseWindow();

    return 0;
}

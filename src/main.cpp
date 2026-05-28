#include "game.hpp"
#include "raylib.h"
#include "renderer.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbit Drift");

    Game game;
    Renderer renderer(game);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        game.update();
        renderer.update(game);

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(renderer.camera);
                renderer.draw3D(game);
            EndMode3D();
            renderer.drawUI(game);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

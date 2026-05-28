#include "scene.hpp"
#include "raylib.h"
#include "renderer.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbit Drift");

    Scene scene;
    Renderer renderer(scene);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        scene.update();
        renderer.update(scene);

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(renderer.camera);
                renderer.draw3D(scene);
            EndMode3D();
            renderer.drawUI(scene);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

#include "scene.hpp"
#include "raylib.h"
#include "render.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbit Drift");

    Scene scene;
    Renderer renderer(&scene);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        scene.update();
        renderer.update();

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(renderer.cam_manager.camera);
                renderer.draw_3d();
            EndMode3D();
            renderer.draw_ui();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

#include "entities.hpp"
#include "game.hpp"
#include "raylib.h"
#include "raymath.h"
#include "renderer.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbit Drift");

    Game game;
    Renderer renderer(game);
    PlayerShip* playerShip = (PlayerShip*)game.getGameObject("player");

    Camera camera = {0};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    float cameraDistance = 3.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        game.update();
        renderer.update(game);

        Vector3 currentPosition = playerShip->getPosition();
        Vector3 forward = playerShip->getForward();
        Vector3 up = playerShip->getUp();

        camera.target = currentPosition;
        Vector3 lookOffset = Vector3Scale(forward, -cameraDistance);
        Vector3 heightOffset = Vector3Scale(up, 2.5f);
        camera.position =
            Vector3Add(Vector3Add(currentPosition, lookOffset), heightOffset);
        camera.up = up;

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);
        renderer.draw(camera.position, game);
        EndMode3D();

        if (playerShip->isInGravitySOI) {
            DrawText("IN GRAVITY", 20, 40, 40, YELLOW);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

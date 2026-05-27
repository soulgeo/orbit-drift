#include "entities.hpp"
#include "game.hpp"
#include "raylib.h"
#include "raymath.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else
#define GLSL_VERSION 100
#endif

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbit Drift");

    Game game;
    PlayerShip* playerShip = (PlayerShip*)game.getGameObject("player");

    Camera camera = {0};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    float cameraDistance = 3.0f;

    SetTargetFPS(60);

    Shader fog =
        LoadShader(TextFormat("resources/shaders/ambient.vert", GLSL_VERSION),
                   TextFormat("resources/shaders/ambient.frag", GLSL_VERSION));

    game.forEachGameObject([&](GameObject& obj) {
        obj.model.materials[0].shader = fog;
    });

    while (!WindowShouldClose()) {
        game.update();

        Vector3 currentPosition = playerShip->getPosition();
        Vector3 forward = playerShip->getForward();
        Vector3 up = playerShip->getUp();

        camera.target = currentPosition;
        Vector3 lookOffset = Vector3Scale(forward, -cameraDistance);
        Vector3 heightOffset = Vector3Scale(up, 2.5f);
        camera.position =
            Vector3Add(Vector3Add(currentPosition, lookOffset), heightOffset);
        camera.up = up;

        int distLoc = GetShaderLocation(fog, "viewPos");
        SetShaderValue(fog, distLoc, &camera.position, SHADER_UNIFORM_VEC3);
        SetShaderValue(fog, fog.locs[SHADER_LOC_VECTOR_VIEW],
                       &camera.position.x, SHADER_UNIFORM_VEC3);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);
        game.draw();
        EndMode3D();

        if (playerShip->isInGravitySOI) {
            DrawText("IN GRAVITY", 20, 40, 40, YELLOW);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

#include "game.hpp"
#include "raylib.h"
#include "raymath.h"
#include "game_objects.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MAX_COLUMNS 20

int main() {
    //================================================================================== 
    // Initialize Window
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbit Drift");

    // Initialize Game
    Game game;
    PlayerShip* playerShip = (PlayerShip*)game.getGameObject("player");

    // Initialize Camera
    Camera camera = {0};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    float cameraDistance = 3.0f;

    SetTargetFPS(60);

    //================================================================================== 
    // Main Loop
    while (!WindowShouldClose())
    {
        game.update();        

        // =============================================================================
        // Camera

        // Extract orientation and tracking data straight from GameObject methods
        Vector3 currentPosition = playerShip->getPosition();
        Vector3 forward = playerShip->getForward();
        Vector3 up = playerShip->getUp();

        // Position camera smoothly behind the target
        camera.target = currentPosition;
        Vector3 lookOffset = Vector3Scale(forward, -cameraDistance);
        Vector3 heightOffset = Vector3Scale(up, 2.5f); 
        camera.position = Vector3Add(Vector3Add(currentPosition, lookOffset), heightOffset);
        camera.up = up; 

        // =============================================================================
        // Draw Scene
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);
                game.draw();
            EndMode3D();
        EndDrawing();
    }
    //================================================================================== 
    // Cleanup resources
    
    CloseWindow();

    return 0;
}

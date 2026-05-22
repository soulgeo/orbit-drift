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

    // Obstacle generation
    float heights[MAX_COLUMNS] = {0};
    Vector3 positions[MAX_COLUMNS] = {0};
    Color colors[MAX_COLUMNS] = {0};

    for (int i = 0; i < MAX_COLUMNS; i++) {
        heights[i] = (float)GetRandomValue(1, 12);
        positions[i] = (Vector3){(float)GetRandomValue(-15, 15), heights[i] / 2.0f, (float)GetRandomValue(-15, 15)};
        colors[i] = (Color){
            (unsigned char)GetRandomValue(20, 255), 
            (unsigned char)GetRandomValue(10, 55), 
            30, 255};
    }

    SetTargetFPS(60);

    //================================================================================== 
    // Main Loop
    while (!WindowShouldClose())
    {
        game.update();        
        
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
                DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f}, DARKGRAY);
                DrawCube((Vector3){-16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f, BLUE);
                DrawCube((Vector3){16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f, LIME);
                DrawCube((Vector3){0.0f, 2.5f, 16.0f}, 32.0f, 5.0f, 1.0f, GOLD);

                for (int i = 0; i < MAX_COLUMNS; i++) {
                    DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
                    DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
                }

                game.draw();

            EndMode3D();
        EndDrawing();
    }
    //================================================================================== 
    // Cleanup resources
    
    CloseWindow();

    return 0;
}

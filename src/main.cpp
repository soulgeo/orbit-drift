#include "raylib.h"
#include <cmath>

#define MAX_COLUMNS 20

int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight, "Orbit Drift");

    Vector2 screenCenter = {screenWidth / 2.0f, screenHeight / 2.0f};

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = {0};
    camera.position = (Vector3){0.0f, 2.0f, 4.0f};  // Camera position
    camera.target = (Vector3){0.0f, 2.0f, 0.0f};    // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};        // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                            // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;         // Camera projection type

    float cameraAngleX = 0.0f;       // Yaw
    float cameraAngleY = 0.5f;       // Pitch
    float cameraDistance = 6.0f;

    // Generates some random columns
    float heights[MAX_COLUMNS] = {0};
    Vector3 positions[MAX_COLUMNS] = {0};
    Color colors[MAX_COLUMNS] = {0};

    for (int i = 0; i < MAX_COLUMNS; i++) {
        heights[i] = (float)GetRandomValue(1, 12);
        positions[i] = (Vector3){
            (float)GetRandomValue(-15, 15), 
            heights[i] / 2.0f, 
            (float)GetRandomValue(-15, 15)
        };
        colors[i] = (Color){
            static_cast<unsigned char>(GetRandomValue(20, 255)), 
            static_cast<unsigned char>(GetRandomValue(10, 55)), 
            30, 255
        };
    }

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        Vector2 mousePosition = GetMousePosition();
        Vector2 mouseDistance = {
            mousePosition.x - screenCenter.x,
            mousePosition.y - screenCenter.y
        };
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) camera.target.z -= 0.1f;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) camera.target.z += 0.1f;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) camera.target.x += 0.1f;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) camera.target.x -= 0.1f;

        cameraAngleX += mouseDistance.x * -0.00005f;
        cameraAngleY += mouseDistance.y * 0.00005f;

        cameraDistance -= GetMouseWheelMove() * 2.0f;

        camera.position.x = camera.target.x + cameraDistance * std::cos(cameraAngleY) * std::sin(cameraAngleX);
        camera.position.y = camera.target.y + cameraDistance * std::sin(cameraAngleY);
        camera.position.z = camera.target.z + cameraDistance * std::cos(cameraAngleY) * std::cos(cameraAngleX);

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);
                DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f}, DARKGRAY);  // Draw ground
                DrawCube((Vector3){-16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f, BLUE);  // Draw a blue wall
                DrawCube((Vector3){16.0f, 2.5f, 0.0f}, 1.0f, 5.0f, 32.0f, LIME);  // Draw a green wall
                DrawCube((Vector3){0.0f, 2.5f, 16.0f}, 32.0f, 5.0f, 1.0f, GOLD);  // Draw a yellow wall

                // Draw some cubes around
                for (int i = 0; i < MAX_COLUMNS; i++) {
                    DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
                    DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
                }

                // Draw player cube
                DrawCube(camera.target, 0.5f, 0.5f, 0.5f, PURPLE);
                DrawCubeWires(camera.target, 0.5f, 0.5f, 0.5f, DARKPURPLE);
            EndMode3D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

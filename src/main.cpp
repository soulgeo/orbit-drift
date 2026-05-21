#include "commands.hpp"
#include "game.hpp"
#include "input_handler.hpp"
#include "raylib.h"
#include "raymath.h"
#include "game_object.hpp"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MAX_COLUMNS 20

struct PlayerShip : public GameObject {
    float forwardSpeed = 0.2f;
    float panSpeed = 0.004f;
    float rollSpeed = 0.02f;
};

int main() {
    //================================================================================== 
    // Initialize Window
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orbit Drift");

    Vector2 screenCenter = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};

    // Initialize Camera
    Camera camera = {0};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    float cameraDistance = 3.0f;

    // Initialize Game
    Game game;
    PlayerShip* playerShip = (PlayerShip*)game.getGameObject("player");

    // Load Player Model
    Mesh coneMesh = GenMeshCone(0.2f, 0.5f, 16);
    Model playerModel = LoadModelFromMesh(coneMesh);
    Matrix baseRotation = MatrixRotateX(-90.0f * DEG2RAD);
    
    // Initialize Input Handler & Define Keybinds
    InputHandler inputHandler;
    
    Command* cmdW = new MoveForwardCommand(playerShip->forwardSpeed);
    Command* cmdS = new MoveBackCommand(playerShip->forwardSpeed);
    Command* cmdA = new MoveLeftCommand(playerShip->forwardSpeed);
    Command* cmdD = new MoveRightCommand(playerShip->forwardSpeed);
    Command* cmdE = new RollCWCommand(playerShip->rollSpeed);
    Command* cmdQ = new RollCCWCommand(playerShip->rollSpeed);

    inputHandler.bindKey(KEY_W, cmdW);
    inputHandler.bindKey(KEY_S, cmdS);
    inputHandler.bindKey(KEY_A, cmdA);
    inputHandler.bindKey(KEY_D, cmdD);
    inputHandler.bindKey(KEY_E, cmdE);
    inputHandler.bindKey(KEY_Q, cmdQ);

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
        // =============================================================================
        // Process Input via Input Handler
        InputHandler::CommandList activeInputs = inputHandler.handleInput();
        for (size_t i = 0; i < activeInputs.count; ++i) {
            activeInputs.commands[i]->execute(*playerShip);
        }

        // =============================================================================
        // Panning / Rotation Update
        Vector2 mousePosition = GetMousePosition();
        Vector2 mouseDistance = { 
            mousePosition.x - screenCenter.x,
            mousePosition.y - screenCenter.y 
        };

        float localYaw   = mouseDistance.x * -playerShip->panSpeed * GetFrameTime();
        float localPitch = mouseDistance.y * -playerShip->panSpeed * GetFrameTime();

        playerShip->rotatePitch(localPitch);
        playerShip->rotateYaw(localYaw);

        // =============================================================================
        // Visuals & Camera Update
        
        // Update model visual rotation matching the ship state
        playerModel.transform = MatrixMultiply(baseRotation, playerShip->transform);

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

                DrawModel(playerModel, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, PURPLE);
                DrawModelWires(playerModel, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, MAROON);
            EndMode3D();
        EndDrawing();
    }

    //================================================================================== 
    // Cleanup resources
    UnloadModel(playerModel);
    
    delete cmdW; delete cmdS; delete cmdA; 
    delete cmdD; delete cmdE; delete cmdQ;

    CloseWindow();

    return 0;
}

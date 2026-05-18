#include <cmath>
#include <string>

#include "raylib.h"
#include "raymath.h"

using namespace std;

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Orbit Drift");

    SetTargetFPS(60);

    Vector2 triangleAnchorPoint = {screenWidth / 2.0f, screenHeight / 2.0f};

    const float speed = 5.0f;

    Vector2 trianglePeak, triangleBaseLeft, triangleBaseRight;

    // Game Loop
    while (!WindowShouldClose()) {

        Vector2 mousePosition = GetMousePosition();
        float angle = Vector2LineAngle(triangleAnchorPoint, mousePosition);

        // Update
        if (IsKeyDown(KEY_W)) triangleAnchorPoint.y -= speed;
        if (IsKeyDown(KEY_S)) triangleAnchorPoint.y += speed;

        if (IsKeyDown(KEY_A)) triangleAnchorPoint.x -= speed;
        if (IsKeyDown(KEY_D)) triangleAnchorPoint.x += speed;

        trianglePeak = {triangleAnchorPoint.x - 35.0f * -cos(angle),
                        triangleAnchorPoint.y - 35.0f * sin(angle)};

        triangleBaseLeft = {triangleAnchorPoint.x - 15.0f * cos(PI/2.0f - angle),
                            triangleAnchorPoint.y - 15.0f * sin(PI/2.0f - angle)};
        triangleBaseRight = {triangleAnchorPoint.x + 15.0f * cos(PI/2.0f - angle),
                             triangleAnchorPoint.y + 15.0f * sin(PI/2.0f - angle)};

        // Draw
        BeginDrawing();
        //////////////////

        ClearBackground(BLACK);
        DrawTriangle(trianglePeak, triangleBaseLeft, triangleBaseRight,
                     RAYWHITE);

        DrawText(("Angle: " + to_string(angle)).c_str(), 50, 30, 16, RAYWHITE);

        DrawText(("Peak x: " + to_string(trianglePeak.x)).c_str(), 50, 50, 16, RAYWHITE);
        DrawText(("Peak y: " + to_string(trianglePeak.y)).c_str(), 50, 70, 16, RAYWHITE);

        DrawText(("Base Left x: " + to_string(triangleBaseLeft.x)).c_str(), 50, 90, 16, RAYWHITE);
        DrawText(("Base Left y: " + to_string(triangleBaseLeft.y)).c_str(), 50, 110, 16, RAYWHITE);

        DrawText(("Base Right x: " + to_string(triangleBaseRight.x)).c_str(), 50, 130, 16, RAYWHITE);
        DrawText(("Base Right y: " + to_string(triangleBaseRight.y)).c_str(), 50, 150, 16, RAYWHITE);

        DrawText(("Anchor Point x: " + to_string(triangleAnchorPoint.x)).c_str(), 50, 170, 16, RAYWHITE);
        DrawText(("Anchor Point y: " + to_string(triangleAnchorPoint.y)).c_str(), 50, 190, 16, RAYWHITE);

        DrawText(("Mouse x: " + to_string(mousePosition.x)).c_str(), 50, 210, 16, RAYWHITE);
        DrawText(("Mouse y: " + to_string(mousePosition.y)).c_str(), 50, 230, 16, RAYWHITE);

        //////////////////
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

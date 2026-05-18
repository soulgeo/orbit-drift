#include "raylib.h"


int main () {
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    const int screenWidth = 800;
    const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "Orbit Drift");

    Vector2 rectPosition = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    Vector2 rectSize = { 50.0f, 50.0f };

    const float speed = 5.0f;

    SetTargetFPS(60);

    // Game Loop
	while (!WindowShouldClose())
	{
        // Update
        if (IsKeyDown(KEY_UP)) rectPosition.y -= speed;
        if (IsKeyDown(KEY_DOWN)) rectPosition.y += speed;

        if (IsKeyDown(KEY_LEFT)) rectPosition.x -= speed;
        if (IsKeyDown(KEY_RIGHT)) rectPosition.x += speed;

        // Draw
		BeginDrawing();
            ClearBackground(BLACK);
            DrawRectangleV(rectPosition, rectSize, MAROON);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

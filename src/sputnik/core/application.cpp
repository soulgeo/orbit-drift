#include "sputnik/core/application.hpp"
#include "raylib.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

namespace Sputnik {

    Application::Application(const char* name) {
        SetConfigFlags(FLAG_MSAA_4X_HINT);

        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, name);
        SetTargetFPS(144);
    }

    Application::~Application() {
        CloseWindow();
    }

    void Application::run() {
        startup();
        engine_.run();
    }

}

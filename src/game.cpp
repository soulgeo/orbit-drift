#include "game.hpp"

#include <memory>
#include <string>
#include <unordered_map>

#include "entities.hpp"
#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else
#define GLSL_VERSION 100
#endif

struct GameImpl {
    std::unordered_map<std::string, std::unique_ptr<GameObject>> gameObjects;
    InputHandler::CommandList activeInputs;
};

Game::Game() {
    impl_ = new GameImpl();
    impl_->gameObjects["player"] = std::make_unique<PlayerShip>();
    impl_->gameObjects["planet1"] = std::make_unique<Planet>(
        PURPLE, (Vector3){30.0f, 5.0f, 30.0f}, 10.0f, 40.0f, 40.0f);
    impl_->gameObjects["planet2"] = std::make_unique<Planet>(
        GREEN, (Vector3){50.0f, 10.0f, -100.0f}, 20.0f, 80.0f, 40.0f);
    impl_->gameObjects["planet3"] = std::make_unique<Planet>(
        YELLOW, (Vector3){-60.0f, -30.0f, 100.0f}, 40.0f, 160.0f, 40.0f);
    impl_->gameObjects["planet4"] = std::make_unique<Planet>(
        BLUE, (Vector3){120.0f, -70.0f, 200.0f}, 5.0f, 20.0f, 40.0f);

    // Keybinds
    inputHandler.bindKey(KEY_SPACE, DOWN, MOVE_UP);
    inputHandler.bindKey(KEY_LEFT_SHIFT, DOWN, MOVE_DOWN);
    inputHandler.bindKey(KEY_W, DOWN, MOVE_FORWARD);
    inputHandler.bindKey(KEY_S, DOWN, MOVE_BACK);
    inputHandler.bindKey(KEY_A, DOWN, MOVE_LEFT);
    inputHandler.bindKey(KEY_D, DOWN, MOVE_RIGHT);
    inputHandler.bindKey(KEY_E, DOWN, ROLL_CW);
    inputHandler.bindKey(KEY_Q, DOWN, ROLL_CCW);
    inputHandler.bindKey(KEY_LEFT_ALT, PRESSED, PAUSE);

    // Load Fog Shader
    fog =
        LoadShader(TextFormat("resources/shaders/ambient.vert", GLSL_VERSION),
                   TextFormat("resources/shaders/ambient.frag", GLSL_VERSION));

    // Assign shader to all game objects
    forEachGameObject(
        [&](GameObject& obj) { obj.model.materials[0].shader = fog; });
}

Game::~Game() {
    UnloadShader(fog);
    delete impl_;
}

GameObject* Game::getGameObject(std::string name) {
    auto it = impl_->gameObjects.find(name);
    if (it != impl_->gameObjects.end()) {
        return it->second.get();
    }
    return nullptr;
}

void Game::forEachGameObject(std::function<void(GameObject&)> func) {
    for (auto& [name, object] : impl_->gameObjects) {
        func(*object);
    }
}

void Game::update() {
    // Handle input
    impl_->activeInputs = inputHandler.handleInput();

    for (size_t i = 0; i < impl_->activeInputs.count; ++i) {
        if (impl_->activeInputs.commands[i] == PAUSE) {
            isPaused = !isPaused;
        }
    }

    if (isPaused) return;

    // Reset ship SOI flag
    PlayerShip* player = (PlayerShip*)getGameObject("player");
    if (player) player->isInGravitySOI = false;

    // Game Object updates
    for (const auto& [name, object] : impl_->gameObjects) {
        object->onUpdate(*this);
    }
}

bool Game::isActiveInput(int input) {
    for (size_t i = 0; i < impl_->activeInputs.count; ++i) {
        if (impl_->activeInputs.commands[i] == input) {
            return true;
        }
    }
    return false;
}

void Game::draw(Vector3 cameraPos) {
    int distLoc = GetShaderLocation(fog, "viewPos");
    SetShaderValue(fog, distLoc, &cameraPos, SHADER_UNIFORM_VEC3);
    SetShaderValue(fog, fog.locs[SHADER_LOC_VECTOR_VIEW], &cameraPos.x,
                   SHADER_UNIFORM_VEC3);

    GameObject* playerShip = getGameObject("player");
    DrawModel(playerShip->model, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f, PURPLE);
    DrawModelWires(playerShip->model, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f,
                   MAROON);

    for (const auto& [name, object] : impl_->gameObjects) {
        if (Planet* planet = dynamic_cast<Planet*>(object.get())) {
            DrawModel(planet->model, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f,
                      planet->color);
            DrawModelWires(
                planet->model, (Vector3){0.0f, 0.0f, 0.0f}, 1.0f,
                ColorTint(planet->color, (Color){200, 200, 200, 255}));
        }
    }
}

#include "game.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include "entities.hpp"
#include "raylib.h"


struct GameImpl {
    std::unordered_map<std::string, std::unique_ptr<GameObject>> gameObjects;
    InputHandler::CommandList activeInputs;
};

Game::Game() {
    impl_ = new GameImpl();
    impl_->gameObjects["player"] = std::make_unique<PlayerShip>();
    impl_->gameObjects["planet1"] = std::make_unique<Planet>(
        PURPLE, (Vector3){30.0f, 5.0f, 30.0f}, 10.0f, 40.0f, 40.0f
    );
    impl_->gameObjects["planet2"] = std::make_unique<Planet>(
        GREEN, (Vector3){50.0f, 10.0f, -100.0f}, 20.0f, 80.0f, 40.0f
    );
    impl_->gameObjects["planet3"] = std::make_unique<Planet>(
        YELLOW, (Vector3){-60.0f, -30.0f, 100.0f}, 40.0f, 160.0f, 40.0f
    );
    impl_->gameObjects["planet4"] = std::make_unique<Planet>(
        BLUE, (Vector3){120.0f, -70.0f, 200.0f}, 5.0f, 20.0f, 40.0f
    );

    // Keybinds
    inputHandler.bindKey(KEY_SPACE, DOWN, "move_up");
    inputHandler.bindKey(KEY_LEFT_SHIFT, DOWN, "move_down");
    inputHandler.bindKey(KEY_W, DOWN, "move_forward");
    inputHandler.bindKey(KEY_S, DOWN, "move_back");
    inputHandler.bindKey(KEY_A, DOWN, "move_left");
    inputHandler.bindKey(KEY_D, DOWN, "move_right");
    inputHandler.bindKey(KEY_E, DOWN, "roll_cw");
    inputHandler.bindKey(KEY_Q, DOWN, "roll_ccw");
    inputHandler.bindKey(KEY_LEFT_ALT, PRESSED, "pause");
}

Game::~Game() {
    delete impl_;
}

GameObject* Game::getGameObject(std::string name){
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

    for (const std::string& action : impl_->activeInputs.commands) {
        if (action == "pause") {
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

bool Game::isActiveInput(std::string input) {
    for (const std::string& action : impl_->activeInputs.commands) {
        if (action == input) {
            return true;
        }
    }
    return false;
}

void Game::draw() {
    GameObject* playerShip = getGameObject("player");
    DrawModel(playerShip->model, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, PURPLE);
    DrawModelWires(playerShip->model, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, MAROON);

    for (const auto& [name, object] : impl_->gameObjects) {
        if (Planet* planet = dynamic_cast<Planet*>(object.get())){
            DrawModel(planet->model, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, planet->color);
            DrawModelWires(planet->model, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, ColorTint(planet->color, (Color){ 200, 200, 200, 255 }));
        }
    }
}

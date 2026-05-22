#include "game.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include "game_objects.hpp"
#include "raylib.h"


struct GameImpl {
    std::unordered_map<std::string, std::unique_ptr<GameObject>> gameObjects;
};

Game::Game() {
    impl_ = new GameImpl();
    impl_->gameObjects["player"] = std::make_unique<PlayerShip>();
    impl_->gameObjects["planet1"] = std::make_unique<Planet>(
        10.0f, (Vector3){30.0f, 5.0f, 30.0f}
    );
    impl_->gameObjects["planet2"] = std::make_unique<Planet>(
        20.0f, (Vector3){50.0f, 10.0f, -100.0f}
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

void Game::update() {
    // Handle input
    InputHandler::CommandList activeInputs = inputHandler.handleInput();

    for (const std::string& action : activeInputs.commands) {
        if (action == "pause") {
            isPaused = !isPaused;
        }
    }

    if (isPaused) return;

    GameObject* controlledObject = getGameObject("player");
    if (controlledObject) {
        for (const std::string& action : activeInputs.commands) {
            controlledObject->handleAction(action);
        }
    }

    // Game Object updates
    for (const auto& [name, object] : impl_->gameObjects) {
        object->onUpdate();
    }
}

void Game::draw() {
    GameObject* playerShip = getGameObject("player");
    DrawModel(playerShip->model, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, PURPLE);
    DrawModelWires(playerShip->model, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, MAROON);

    for (const auto& [name, object] : impl_->gameObjects) {
        if (Planet* planet = dynamic_cast<Planet*>(object.get())){
            DrawModel(object->model, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, GREEN);
            DrawModelWires(object->model, (Vector3){ 0.0f, 0.0f, 0.0f }, 1.0f, DARKGREEN);
        }
    }
}

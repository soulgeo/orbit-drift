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

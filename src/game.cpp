#include "game.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include "game_objects.hpp"

struct GameImpl {
    std::unordered_map<std::string, std::unique_ptr<GameObject>> gameObjects;
};

Game::Game() {
    impl_ = new GameImpl();
    impl_->gameObjects["player"] = std::make_unique<PlayerShip>();

    // Input
    inputHandler.bindKey(KEY_SPACE, "move_up");
    inputHandler.bindKey(KEY_LEFT_SHIFT, "move_down");
    inputHandler.bindKey(KEY_W, "move_forward");
    inputHandler.bindKey(KEY_S, "move_back");
    inputHandler.bindKey(KEY_A, "move_left");
    inputHandler.bindKey(KEY_D, "move_right");
    inputHandler.bindKey(KEY_E, "roll_cw");
    inputHandler.bindKey(KEY_Q, "roll_ccw");
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
    InputHandler::CommandList activeInputs = inputHandler.handleInput();
    GameObject* controlledObject = getGameObject("player");

    if (controlledObject) {
        for (const std::string& action : activeInputs.commands) {
            controlledObject->handleAction(action);
        }
    }

    for (const auto& [name, object] : impl_->gameObjects) {
        object->onUpdate();
    }
}

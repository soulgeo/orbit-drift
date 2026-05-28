#include "scene.hpp"

#include <memory>
#include <string>
#include <unordered_map>

#include "entities.hpp"
#include "raylib.h"

struct SceneImpl {
    std::unordered_map<std::string, std::unique_ptr<GameObject>> gameObjects;
    InputHandler::CommandList activeInputs;
};

Scene::Scene() {
    impl_ = new SceneImpl();
    impl_->gameObjects["player"] = std::make_unique<PlayerShip>();
    impl_->gameObjects["planet1"] = std::make_unique<Planet>(
        (Vector3){30.0f, 5.0f, 30.0f}, 10.0f, 40.0f, 40.0f);
    impl_->gameObjects["planet2"] = std::make_unique<Planet>(
        (Vector3){50.0f, 10.0f, -100.0f}, 20.0f, 80.0f, 40.0f);
    impl_->gameObjects["planet3"] = std::make_unique<Planet>(
        (Vector3){-60.0f, -30.0f, 100.0f}, 40.0f, 160.0f, 40.0f);
    impl_->gameObjects["planet4"] = std::make_unique<Planet>(
        (Vector3){120.0f, -70.0f, 200.0f}, 5.0f, 20.0f, 40.0f);

    inputHandler.bindKey(KEY_SPACE, DOWN, MOVE_UP);
    inputHandler.bindKey(KEY_LEFT_SHIFT, DOWN, MOVE_DOWN);
    inputHandler.bindKey(KEY_W, DOWN, MOVE_FORWARD);
    inputHandler.bindKey(KEY_S, DOWN, MOVE_BACK);
    inputHandler.bindKey(KEY_A, DOWN, MOVE_LEFT);
    inputHandler.bindKey(KEY_D, DOWN, MOVE_RIGHT);
    inputHandler.bindKey(KEY_E, DOWN, ROLL_CW);
    inputHandler.bindKey(KEY_Q, DOWN, ROLL_CCW);
    inputHandler.bindKey(KEY_LEFT_ALT, PRESSED, PAUSE);
}

Scene::~Scene() {
    delete impl_;
}

GameObject* Scene::getGameObject(const std::string& name) {
    auto it = impl_->gameObjects.find(name);
    if (it != impl_->gameObjects.end()) {
        return it->second.get();
    }
    return nullptr;
}

void Scene::forEachGameObject(std::function<void(const std::string&, GameObject&)> func) {
    for (auto& [name, object] : impl_->gameObjects) {
        func(name, *object);
    }
}

void Scene::update() {
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

bool Scene::isActiveInput(int input) {
    for (size_t i = 0; i < impl_->activeInputs.count; ++i) {
        if (impl_->activeInputs.commands[i] == input) {
            return true;
        }
    }
    return false;
}

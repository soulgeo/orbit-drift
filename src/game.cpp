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

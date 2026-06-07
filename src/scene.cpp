#include "scene.hpp"

#include <memory>
#include <string>
#include <unordered_map>

#include "entities.hpp"
#include "raylib.h"

struct SceneImpl {
    std::unordered_map<std::string, std::unique_ptr<GameObject>> game_objects;
};

Scene::Scene() {
    impl_ = new SceneImpl();
    impl_->game_objects["player"] = std::make_unique<PlayerShip>();
    impl_->game_objects["planet1"] = std::make_unique<Planet>(
        (Vector3){-30.0f, 5.0f, -150.0f}, 10.0f, 40.0f, 40.0f);
    impl_->game_objects["planet2"] = std::make_unique<Planet>(
        (Vector3){100.0f, 10.0f, -100.0f}, 20.0f, 80.0f, 40.0f);
    impl_->game_objects["planet3"] = std::make_unique<Planet>(
        (Vector3){-200.0f, -30.0f, 100.0f}, 40.0f, 160.0f, 40.0f);
    impl_->game_objects["planet4"] = std::make_unique<Planet>(
        (Vector3){120.0f, -70.0f, 200.0f}, 5.0f, 20.0f, 40.0f);
}

Scene::~Scene() {
    delete impl_;
}

GameObject* Scene::get_game_object(const std::string& name) {
    auto it = impl_->game_objects.find(name);
    if (it != impl_->game_objects.end()) {
        return it->second.get();
    }
    return nullptr;
}

void Scene::for_each_game_object(std::function<void(const std::string&, GameObject&)> func) {
    for (auto& [name, object] : impl_->game_objects) {
        func(name, *object);
    }
}


#include "sputnik/ecs/scene.hpp"

#include "sputnik/core/engine.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "game/planet_factory.hpp"
#include <raylib.h>
#include <raymath.h>
#include "sputnik/rendering/renderer.hpp"

namespace Sputnik {

    Scene::Scene(Engine* engine) {
        engine_ = engine;
    }

    Scene::~Scene() = default;

    GameObject* Scene::game_object(const std::string& name) {
        auto it = game_objects_.find(name);
        if (it != game_objects_.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void Scene::add_game_object(std::string name, std::unique_ptr<GameObject> game_object) {
        game_objects_[name] = std::move(game_object);
    }

    void Scene::for_each_game_object(std::function<void(const std::string&, GameObject&)> func) {
        for (auto& [name, object] : game_objects_) {
            if (name != "camera_body") func(name, *object);
        }

        auto it = game_objects_.find("camera_body");
        if (it != game_objects_.end()) {
            func(it->first, *it->second);
        }
    }

}

#include "sputnik/ecs/scene.hpp"

#include "audio/audio_listener_component.hpp"
#include "ecs/game_object.hpp"
#include "global_control_component.hpp"
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
        auto& renderer = engine->renderer();
        auto& event_dsp = engine->event_dsp();

        auto camera_body = std::make_unique<GameObject>(engine);
        camera_body->add_component<CameraComponent>(this, &renderer, &event_dsp);
        camera_body->add_component<GlobalControlComponent>();
        camera_body->add_component<AudioListenerComponent>();
        
        add_game_object("camera_body", std::move(camera_body));
    }

    Scene::~Scene() = default;

    GameObject* Scene::game_object(std::string_view name) {
        auto it = name_index_.find(std::string(name));
        if (it != name_index_.end()) {
            return it->second;
        }
        return nullptr;
    }

    void Scene::add_game_object(std::string_view name, std::unique_ptr<GameObject> game_object) {
        name_index_[std::string(name)] = game_object.get();
        game_objects_.push_back(std::move(game_object));
    }

    size_t Scene::game_object_count() const {
        return game_objects_.size();
    }

    GameObject* Scene::game_object_at(size_t index) const {
        if (index < game_objects_.size()) {
            return game_objects_[index].get();
        }
        return nullptr;
    }

}

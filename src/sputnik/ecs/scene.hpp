#pragma once

#include "sputnik/ecs/game_object.hpp"
#include <memory>
#include <string_view>
#include <unordered_map>
#include <string>
#include <vector>

namespace Sputnik {

    class Engine;
    class Renderer;
    class Physics;

    class Scene {
        std::vector<std::unique_ptr<GameObject>> game_objects_;
        std::unordered_map<std::string, GameObject*> name_index_;
        Engine* engine_;

    public:
        Scene(Engine* engine);
        ~Scene();

        static constexpr size_t MAX_GAMEOBJECT_COUNT = 200;

        GameObject* game_object(std::string_view name);
        void add_game_object(std::string_view name, std::unique_ptr<GameObject> game_object);

        size_t game_object_count() const;
        GameObject* game_object_at(size_t index) const;
    };
}

#pragma once

#include "sputnik/ecs/game_object.hpp"
#include <unordered_map>
#include <string>
#include <functional>

namespace Sputnik {

    class Engine;
    class Renderer;
    class Physics;

    class Scene {
        std::unordered_map<std::string, std::unique_ptr<GameObject>> game_objects_;
        Engine* engine_;

    public:
        Scene(Engine* engine);
        ~Scene();

        static constexpr size_t MAX_GAMEOBJECT_COUNT = 200;

        GameObject* game_object(const std::string& name);
        void add_game_object(std::string name, std::unique_ptr<GameObject> game_object);

        void for_each_game_object(std::function<void(const std::string&, GameObject&)> func);
    };

}

#ifndef SCENE_HPP
#define SCENE_HPP

#include "sputnik/ecs/game_object.hpp"
#include "sputnik/rendering/renderer.hpp"
#include "sputnik/core/resource_manager.hpp"
#include <unordered_map>
#include <string>
#include <functional>

namespace Sputnik {

    class Scene {
        std::unordered_map<std::string, std::unique_ptr<GameObject>> game_objects_;
        ResourceManager* rsrc_manager_;
        Engine* engine_;

    public:
        Scene(Engine* engine, Renderer* renderer, class Physics* physics, ResourceManager* rsrc_manager);
        ~Scene();

        static constexpr size_t MAX_GAMEOBJECT_COUNT = 200;

        GameObject* get_game_object(const std::string& name);
        void for_each_game_object(std::function<void(const std::string&, GameObject&)> func);
    };

}

#endif // SCENE_HPP

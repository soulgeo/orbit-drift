#ifndef SCENE_HPP
#define SCENE_HPP

#include "game_object.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include <unordered_map>
#include <string>
#include <functional>

class Scene {
    std::unordered_map<std::string, std::unique_ptr<GameObject>> game_objects_;
    ResourceManager* rsrc_manager_;
    Engine* engine_;

public:
    Scene(Engine* engine, Renderer* renderer, ResourceManager* rsrc_manager);
    ~Scene();

    static constexpr size_t MAX_GAMEOBJECT_COUNT = 200;

    GameObject* get_game_object(const std::string& name);
    void for_each_game_object(std::function<void(const std::string&, GameObject&)> func);
};

#endif // SCENE_HPP

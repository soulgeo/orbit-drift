#ifndef SCENE_HPP
#define SCENE_HPP

#include "game_object.hpp"
#include "render.hpp"
#include "resource_manager.hpp"
#include <string>
#include <functional>


struct SceneImpl;

class Scene {
public:
    Scene(Renderer* renderer, ResourceManager* rsrc_manager);
    ~Scene();

    static constexpr size_t MAX_GAMEOBJECT_COUNT = 200;

    GameObject* get_game_object(const std::string& name);
    void for_each_game_object(std::function<void(const std::string&, GameObject&)> func);

private:
    SceneImpl* impl_;
    ResourceManager* rsrc_manager_;
};

#endif // SCENE_HPP

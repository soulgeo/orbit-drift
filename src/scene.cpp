#include "scene.hpp"

#include <memory>
#include <string>
#include <unordered_map>

#include "camera.hpp"
#include "entities.hpp"
#include "planet_factory.hpp"
#include "raylib.h"
#include "raymath.h"
#include "renderable.hpp"
#include "render.hpp"
#include "resource_manager.hpp"

struct SceneImpl {
    std::unordered_map<std::string, std::unique_ptr<GameObject>> game_objects;
};

Scene::Scene(Renderer* renderer, ResourceManager* rsrc_manager) 
    : rsrc_manager_(rsrc_manager) 
{
    impl_ = new SceneImpl();

    auto& player = impl_->game_objects["player"];
    player = std::make_unique<PlayerShip>();
    auto player_model = rsrc_manager_->LoadModel("resources/models/scene.gltf");
    auto texture = rsrc_manager_->LoadTexture("resources/models/textures/Material_baseColor.png");
    player_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    player_model.transform *= MatrixRotateX(180.0f * DEG2RAD) 
        * MatrixRotateZ(180.0f * DEG2RAD) 
        * MatrixScale(0.2f, 0.2f, 0.2f);
    auto player_rend = std::make_unique<Renderable>(player.get(), renderer, player_model);
    player_rend->set_initial_transform(player_model.transform);
    player_rend->set_color(RAYWHITE);
    player->add_renderable(std::move(player_rend));

    auto planet_factory = PlanetFactory(renderer, rsrc_manager);
    impl_->game_objects["planet1"] = planet_factory.create(
        (Vector3){-30.0f, 5.0f, -150.0f}, 10.0f, 40.0f, 40.0f, PURPLE);
    impl_->game_objects["planet2"] = planet_factory.create(
        (Vector3){100.0f, 10.0f, -100.0f}, 20.0f, 80.0f, 40.0f, GREEN);
    impl_->game_objects["planet3"] = planet_factory.create(
        (Vector3){-200.0f, -30.0f, 100.0f}, 40.0f, 160.0f, 40.0f, YELLOW);
    impl_->game_objects["planet4"] = planet_factory.create(
        (Vector3){120.0f, -70.0f, 200.0f}, 5.0f, 20.0f, 40.0f, BLUE);

    auto camera_body = std::make_unique<CameraBody>(this, renderer);
    impl_->game_objects["camera_body"] = std::move(camera_body);
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
        if (name != "camera_body") func(name, *object);
    }

    auto it = impl_->game_objects.find("camera_body");
    if (it != impl_->game_objects.end()) {
        func(it->first, *it->second);
    }
}


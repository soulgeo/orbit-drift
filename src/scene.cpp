#include "scene.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "camera_component.hpp"
#include "control_component.hpp"
#include "debug_component.hpp"
#include "physics_component.hpp"
#include "planet_factory.hpp"
#include "raylib.h"
#include "raymath.h"
#include "renderable_component.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"

Scene::Scene(Engine* engine, Renderer* renderer, ResourceManager* rsrc_manager) 
{
    rsrc_manager_ = rsrc_manager;
    engine_ = engine;

    auto& player = game_objects_["player"];
    player = std::make_unique<GameObject>(engine);

    auto player_model = rsrc_manager_->load_model("resources/models/scene.gltf");
    auto texture = rsrc_manager_->load_texture("resources/models/textures/Material_baseColor.png");
    player_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    player_model.transform *= MatrixRotateX(180.0f * DEG2RAD) 
        * MatrixRotateZ(180.0f * DEG2RAD) 
        * MatrixScale(0.2f, 0.2f, 0.2f);
    auto player_rend = std::make_unique<RenderableComponent>(player.get(), renderer, player_model);
    player_rend->set_initial_transform(player_model.transform);
    player_rend->set_color(RAYWHITE);

    player->add_component(std::move(player_rend));

    auto player_physics = std::make_unique<PhysicsComponent>(
        player.get(), Vector3Zero(), Vector3Zero(), 10.0f);    
    player_physics->set_drag(4.0f);
    player->add_component(std::move(player_physics));
    player->add_component(std::make_unique<ControlComponent>(player.get()));
    player->add_component(std::make_unique<DebugComponent>(player.get(), renderer));

    auto planet_factory = PlanetFactory(engine, renderer, rsrc_manager);
    game_objects_["planet1"] = planet_factory.create(
        (Vector3){-30.0f, 5.0f, -150.0f}, 10.0f, 40.0f, 40.0f, PURPLE);
    game_objects_["planet2"] = planet_factory.create(
        (Vector3){100.0f, 10.0f, -100.0f}, 20.0f, 80.0f, 40.0f, GREEN);
    game_objects_["planet3"] = planet_factory.create(
        (Vector3){-200.0f, -30.0f, 100.0f}, 40.0f, 160.0f, 40.0f, YELLOW);
    game_objects_["planet4"] = planet_factory.create(
        (Vector3){120.0f, -70.0f, 200.0f}, 5.0f, 20.0f, 40.0f, BLUE);

    auto& camera_body = game_objects_["camera_body"];
    camera_body = std::make_unique<GameObject>(engine);
    camera_body->add_component(std::make_unique<CameraComponent>(camera_body.get(), this, renderer));
    camera_body->add_component(std::make_unique<DebugComponent>(camera_body.get(), renderer));
}

Scene::~Scene() = default;

GameObject* Scene::get_game_object(const std::string& name) {
    auto it = game_objects_.find(name);
    if (it != game_objects_.end()) {
        return it->second.get();
    }
    return nullptr;
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

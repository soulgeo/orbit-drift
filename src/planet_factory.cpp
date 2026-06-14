#include "planet_factory.hpp"
#include "game_object.hpp"
#include "gravity_component.hpp"
#include "raylib.h"
#include "renderable_component.hpp"
#include "collider_component.hpp"
#include "raymath.h"
#include "renderer.hpp"
#include "transform_component.hpp"
#include <memory>

PlanetFactory::PlanetFactory(Engine* engine, Renderer* renderer, Physics* physics, ResourceManager* rsrc_manager) : 
    engine_(engine), renderer_(renderer), physics_(physics), rsrc_manager_(rsrc_manager) {}

std::unique_ptr<GameObject> PlanetFactory::create(
    Vector3 position, 
    float radius, 
    float gravity_radius, 
    float gravity_force_amp,
    Color color
) {
    auto planet = std::make_unique<GameObject>(engine_);
    planet->get_component<TransformComponent>()->set_position(position);

    auto planet_gravity = std::make_unique<GravityComponent>(
            planet.get(), gravity_radius, gravity_force_amp
        );
    
    auto planet_collider = std::make_unique<ColliderComponent>(
        planet.get(), physics_, Vector3Zero(), gravity_radius, true
    );

    auto model = LoadModelFromMesh(GenMeshSphere(1.0f, 30.0f, 30.0f));
    model.transform *= MatrixScale(radius, radius, radius);
    auto planet_rend = std::make_unique<RenderableComponent>(
        planet.get(), renderer_, model
    );
    planet_rend->set_initial_transform(model.transform);
    planet_rend->set_color(color);

    auto gravity_model = LoadModelFromMesh(GenMeshSphere(1.0f, 30.0f, 30.0f));
    gravity_model.transform *= 
        MatrixScale(gravity_radius, gravity_radius, gravity_radius);
    auto planet_gravity_rend = std::make_unique<RenderableComponent>(
        planet.get(), renderer_, gravity_model
    );
    planet_gravity_rend->set_initial_transform(gravity_model.transform);
    planet_gravity_rend->set_color(GRAY);
    planet_gravity_rend->set_alpha(0.1f);
    planet_gravity_rend->set_draw_wires(true);

    planet->add_component(std::move(planet_rend));
    planet->add_component(std::move(planet_gravity_rend));
    planet->add_component(std::move(planet_gravity));
    planet->add_component(std::move(planet_collider));
    planet->add_component(std::make_unique<DebugComponent>(planet.get(), renderer_));

    return std::move(planet);
}

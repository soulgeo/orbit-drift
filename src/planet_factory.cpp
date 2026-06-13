#include "planet_factory.hpp"
#include "game_object.hpp"
#include "gravity_component.hpp"
#include "renderable_component.hpp"
#include "raymath.h"
#include "renderer.hpp"
#include "transform_component.hpp"
#include <memory>

PlanetFactory::PlanetFactory(Engine* engine, Renderer* renderer, ResourceManager* rsrc_manager) : 
    engine_(engine), renderer_(renderer), rsrc_manager_(rsrc_manager) {}

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

    auto model = LoadModelFromMesh(GenMeshSphere(1.0f, 30.0f, 30.0f));
    model.transform *= MatrixScale(radius, radius, radius);
    auto planet_rend = std::make_unique<RenderableComponent>(
        planet.get(), renderer_, model
    );
    planet_rend->set_initial_transform(model.transform);
    planet_rend->set_color(color);

    planet->add_component(std::move(planet_rend));
    planet->add_component(std::move(planet_gravity));
    planet->add_component(std::make_unique<DebugComponent>(planet.get(), renderer_));

    return std::move(planet);
}

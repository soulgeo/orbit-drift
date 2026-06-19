#include "planet_factory.hpp"
#include <sputnik.hpp>
#include "gravity_component.hpp"
#include <raylib.h>
#include <raymath.h>
#include <memory>

using namespace Sputnik;

PlanetFactory::PlanetFactory(Engine* engine) : 
    engine_(engine) {
    renderer_ = &engine->renderer();
    physics_ = &engine->physics();
    rsrc_manager_ = &engine->resource_manager();
    event_dsp_ = &engine->event_dsp();
}

std::unique_ptr<GameObject> PlanetFactory::create(
    Vector3 position, 
    float radius, 
    float gravity_radius, 
    float gravity_force_amp,
    Color color
) {
    auto planet = std::make_unique<GameObject>(engine_);
    planet->component<TransformComponent>()->set_position(position);

    auto planet_gravity = planet->add_component<GravityComponent>(
        gravity_radius, gravity_force_amp
    );
    
    auto planet_collider = planet->add_component<ColliderComponent>(
        physics_, Vector3Zero(), gravity_radius, true
    );

    auto model = LoadModelFromMesh(GenMeshSphere(1.0f, 48.0f, 48.0f));
    model.transform *= MatrixScale(radius, radius, radius);
    auto planet_rend = planet->add_component<RenderableComponent>(renderer_, model);
    planet_rend->set_initial_transform(model.transform);
    planet_rend->set_color(color);

    auto gravity_model = LoadModelFromMesh(GenMeshSphere(1.0f, 48.0f, 48.0f));
    gravity_model.transform *= 
        MatrixScale(gravity_radius, gravity_radius, gravity_radius);

    // auto planet_gravity_rend = planet->add_component<RenderableComponent>(
    //     renderer_, gravity_model
    // );
    // planet_gravity_rend->set_initial_transform(gravity_model.transform);
    // planet_gravity_rend->set_color(GRAY);
    // planet_gravity_rend->set_alpha(0.1f);
    // planet_gravity_rend->set_draw_wires(true);

    return std::move(planet);
}

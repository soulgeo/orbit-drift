#include "planet_factory.hpp"
#include "renderable.hpp"

std::unique_ptr<Planet> PlanetFactory::create(
    Vector3 position, 
    float radius, 
    float gravity_radius, 
    float gravity_force,
    Color color
) {
    auto planet = std::make_unique<Planet>(position, radius, gravity_radius, gravity_force);
    auto model = LoadModelFromMesh(GenMeshSphere(1.0f, 30.0f, 30.0f));
    model.transform *= MatrixScale(radius, radius, radius);
    auto planet_rend = std::make_unique<Renderable>(planet.get(), renderer_, model);
    planet_rend->set_color(color);
    planet->add_renderable(std::move(planet_rend));

    return std::move(planet);
}

#include "sputnik/ecs/game_object.hpp"
#include "sputnik/physics/physics.hpp"
#include "sputnik/rendering/renderer.hpp"
#include "sputnik/core/resource_manager.hpp"
#include <memory>

class PlanetFactory {
    Sputnik::Renderer* renderer_;
    Sputnik::ResourceManager* rsrc_manager_;
    Sputnik::Engine* engine_;
    Sputnik::Physics* physics_;

public:
    PlanetFactory(Sputnik::Engine* engine, Sputnik::Renderer* renderer, Sputnik::Physics* physics, Sputnik::ResourceManager* rsrc_manager);
    ~PlanetFactory() {};

    std::unique_ptr<Sputnik::GameObject> create(
        Vector3 position, 
        float radius, 
        float gravity_radius, 
        float gravity_force,
        Color color
    );
};

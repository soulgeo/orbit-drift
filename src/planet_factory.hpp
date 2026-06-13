#include "game_object.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include <memory>

class PlanetFactory {
    Renderer* renderer_;
    ResourceManager* rsrc_manager_;
    Engine* engine_;

public:
    PlanetFactory(Engine* engine, Renderer* renderer, ResourceManager* rsrc_manager);
    ~PlanetFactory() {};

    std::unique_ptr<GameObject> create(
        Vector3 position, 
        float radius, 
        float gravity_radius, 
        float gravity_force,
        Color color
    );
};

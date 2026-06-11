#include "entities.hpp"
#include "render.hpp"
#include "resource_manager.hpp"
#include <memory>

class PlanetFactory {
public:
    PlanetFactory(Renderer* renderer, ResourceManager* rsrc_manager)
    : renderer_(renderer), rsrc_manager_(rsrc_manager) {}

    ~PlanetFactory() {};
    std::unique_ptr<Planet> create(
        Vector3 position, 
        float radius, 
        float gravity_radius, 
        float gravity_force,
        Color color
    );

private:
    Renderer* renderer_;
    ResourceManager* rsrc_manager_;
};

#include "events/event_dispatcher.hpp"
#include <sputnik.hpp>
#include <memory>

class PlanetFactory {
    Sputnik::Renderer* renderer_;
    Sputnik::ResourceManager* rsrc_manager_;
    Sputnik::Engine* engine_;
    Sputnik::Physics* physics_;
    Sputnik::EventDispatcher* event_dsp_;

public:
    PlanetFactory(Sputnik::Engine* engine);
    ~PlanetFactory() {};

    std::unique_ptr<Sputnik::GameObject> create(
        Vector3 position, 
        float radius, 
        float gravity_radius, 
        float gravity_force,
        Color color
    );
};

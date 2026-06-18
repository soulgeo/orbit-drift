#include "game/setup_game.hpp"

namespace OrbitDrift {
    class Application : public Sputnik::Application {
    public:
        Application() : Sputnik::Application("Orbit Drift") {}

        virtual void startup() override {
            bind_inputs(*engine_);
            auto scene = create_main_scene(engine_.get());
            engine_->set_scene(std::move(scene));
        }
    };
}

namespace Sputnik {
    std::unique_ptr<Application> create_application() {
        return std::make_unique<OrbitDrift::Application>();
    }
}

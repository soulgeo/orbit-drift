#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "sputnik/core/input_handler.hpp"
#include "sputnik/physics/physics.hpp"
#include "sputnik/rendering/renderer.hpp"
#include "sputnik/core/resource_manager.hpp"
#include "sputnik/ecs/scene.hpp"
#include <memory>

namespace Sputnik {

    class Physics;

    class Engine {
        Renderer renderer_;
        Physics physics_;
        ResourceManager rsrc_manager_;
        InputHandler input_handler_;

        std::unique_ptr<Scene> scene_;
        bool is_running_;
        bool is_paused_;
        float dt_;
        float fixed_dt_ = 0.005f;
        float accumulator_ = 0.0f;

        void start();

        void process_input();
        void update();
        void render();

    public:
        Engine();
        ~Engine();

        Renderer& renderer();
        ResourceManager& resource_manager();
        Physics& physics();
        InputHandler& input_handler();

        void toggle_pause();
        bool is_paused() const;

        float dt() const;
        float fixed_dt() const;
        float interpolation_alpha() const;
        Scene& scene() const;
        void set_scene(std::unique_ptr<Scene> scene);

        void run();
    };

}

#endif // !ENGINE_HPP

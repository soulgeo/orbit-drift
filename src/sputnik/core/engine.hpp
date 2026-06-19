#pragma once

#include "events/event_dispatcher.hpp"
#include "sputnik/core/input_handler.hpp"
#include "sputnik/physics/physics.hpp"
#include "sputnik/rendering/renderer.hpp"
#include "sputnik/core/resource_manager.hpp"
#include "sputnik/ecs/scene.hpp"
#include <memory>

namespace Sputnik {

    class Physics;

    class Engine {
        Physics physics_;
        ResourceManager resource_mgr_;
        Renderer renderer_;
        InputHandler input_handler_;
        EventDispatcher event_dsp_;

        std::unique_ptr<Scene> scene_;
        bool is_running_;
        bool is_paused_;
        float dt_;                   // Delta Time between frames
        float fixed_dt_ = 0.005f;    // Delta Time between fixed updates
        float accumulator_ = 0.0f;

        void init();

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
        EventDispatcher& event_dsp();

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

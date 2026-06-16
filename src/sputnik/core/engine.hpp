#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "sputnik/core/input_handler.hpp"
#include "sputnik/physics/physics.hpp"
#include "sputnik/rendering/renderer.hpp"
#include "sputnik/core/resource_manager.hpp"
#include "sputnik/ecs/scene.hpp"
#include <memory>

namespace Sputnik {

    enum UserInput {
        INPUT_NONE = 0,
        INPUT_PAUSE,
        INPUT_DEBUG,
        INPUT_MOVE_UP,
        INPUT_MOVE_DOWN,
        INPUT_MOVE_FORWARD,
        INPUT_MOVE_BACK,
        INPUT_MOVE_LEFT,
        INPUT_MOVE_RIGHT,
        INPUT_ROLL_CW,
        INPUT_ROLL_CCW,
    };

    class Physics;

    class Engine {
        Renderer renderer_;
        Physics physics_;
        ResourceManager rsrc_manager_;

        InputHandler::CommandList active_inputs_;

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

        const Renderer& renderer() const;
        const ResourceManager& resource_manager() const;
        const Physics& physics() const;

        InputHandler input_handler;
        bool is_active_input(int input);

        float get_dt() const;
        float get_fixed_dt() const;
        float get_interpolation_alpha() const;
        Scene& get_scene() const;

        void load_scene();
        void run();
    };

}

#endif // !ENGINE_HPP

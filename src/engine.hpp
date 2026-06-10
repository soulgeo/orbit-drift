#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "input_handler.hpp"
#include "raylib.h"
#include "render.hpp"
#include "resource_manager.hpp"
#include "scene.hpp"
#include <memory>

enum UserInput {
    INPUT_NONE = 0,
    INPUT_PAUSE,
    INPUT_MOVE_UP,
    INPUT_MOVE_DOWN,
    INPUT_MOVE_FORWARD,
    INPUT_MOVE_BACK,
    INPUT_MOVE_LEFT,
    INPUT_MOVE_RIGHT,
    INPUT_ROLL_CW,
    INPUT_ROLL_CCW,
};

class Engine {
public:
    Engine();

    InputHandler input_handler;
    bool is_active_input(int input);

    float get_dt() const {
        return dt_;
    }

    float get_fixed_dt() const {
        return fixed_dt_;
    }

    Scene& get_scene() const {
        return *scene_;
    }

    void run() {
        while (is_running_ && !WindowShouldClose()) {
            process_input();
            update();
            render();
        }
    };

private:
    InputHandler::CommandList active_inputs;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<Scene> scene_;
    std::unique_ptr<ResourceManager> rsrc_manager_;
    bool is_running_;
    bool is_paused_;
    float dt_;
    float fixed_dt_ = 0.01f;
    float accumulator_ = 0.0f;

    void process_input();
    void update();
    void render();
};

#endif // !ENGINE_HPP

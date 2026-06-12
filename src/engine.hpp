#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "input_handler.hpp"
#include "renderer.hpp"
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
    struct Impl;
    std::unique_ptr<Impl> impl_;

    void process_input();
    void update();
    void render();
public:
    Engine();
    ~Engine();

    InputHandler input_handler;
    bool is_active_input(int input);

    float get_dt() const;
    float get_fixed_dt() const;
    float get_interpolation_alpha() const;
    Scene& get_scene() const;

    void run();
};

#endif // !ENGINE_HPP

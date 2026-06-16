#include "engine.hpp"
#include "game_object.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include <memory>
#include <string>
#include "timer.hpp"
#include "physics.hpp"

Engine::Engine() : 
    renderer_(Renderer()), physics_(Physics()), rsrc_manager_(ResourceManager())
{
    is_running_ = true;
    is_paused_ = false;

    input_handler.bind_key(KEY_SPACE, DOWN, INPUT_MOVE_UP);
    input_handler.bind_key(KEY_LEFT_SHIFT, DOWN, INPUT_MOVE_DOWN);
    input_handler.bind_key(KEY_W, DOWN, INPUT_MOVE_FORWARD);
    input_handler.bind_key(KEY_S, DOWN, INPUT_MOVE_BACK);
    input_handler.bind_key(KEY_A, DOWN, INPUT_MOVE_LEFT);
    input_handler.bind_key(KEY_D, DOWN, INPUT_MOVE_RIGHT);
    input_handler.bind_key(KEY_E, DOWN, INPUT_ROLL_CW);
    input_handler.bind_key(KEY_Q, DOWN, INPUT_ROLL_CCW);
    input_handler.bind_key(KEY_LEFT_ALT, PRESSED, INPUT_PAUSE);
    input_handler.bind_key(KEY_F3, PRESSED, INPUT_DEBUG);
}

Engine::~Engine() = default;

const Renderer& Engine::renderer() const {
    return renderer_;
}

const ResourceManager& Engine::resource_manager() const {
    return rsrc_manager_;
}

const Physics& Engine::physics() const {
    return physics_;
}

bool Engine::is_active_input(int input) {
    for (size_t i = 0; i < active_inputs_.count; ++i) {
        if (active_inputs_.commands[i] == input) {
            return true;
        }
    }
    return false;
}

float Engine::get_dt() const {
    return dt_;
}

float Engine::get_fixed_dt() const {
    return fixed_dt_;
}

float Engine::get_interpolation_alpha() const {
    return accumulator_ / fixed_dt_;
}

Scene& Engine::get_scene() const {
    return *scene_;
}

void Engine::run() {
    start();
    while (is_running_ && !WindowShouldClose()) {
        process_input();
        update();
        render();
    }
}

void Engine::start() {
    scene_->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.start();
    });
}

void Engine::process_input() {
    active_inputs_ = input_handler.handle_input();

    for (size_t i = 0; i < active_inputs_.count; ++i) {
        if (active_inputs_.commands[i] == INPUT_PAUSE) {
            is_paused_ = !is_paused_;
        }
    }
}


void Engine::update() {
    if (is_paused_) return;

    Timer timer("Engine::update");

    scene_->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.early_update();
    });

    dt_ = GetFrameTime();
    accumulator_ += dt_;
    while (accumulator_ >= fixed_dt_){
        scene_->for_each_game_object([this](const std::string& s, GameObject& obj){
            obj.fixed_update();
        });
        physics_.update(this);
        accumulator_ -= fixed_dt_;
    }

    scene_->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.update();
    });

    scene_->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.late_update();
    });

}

void Engine::render() {
    renderer_.render(*this);
}

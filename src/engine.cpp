#include "engine.hpp"
#include "game_object.hpp"
#include "resource_manager.hpp"
#include <memory>

Engine::Engine() : is_running_(true), is_paused_(false) {
    renderer_ = std::make_unique<Renderer>();
    rsrc_manager_ = std::make_unique<ResourceManager>();
    scene_ = std::make_unique<Scene>(renderer_.get(), rsrc_manager_.get());

    input_handler.bind_key(KEY_SPACE, DOWN, INPUT_MOVE_UP);
    input_handler.bind_key(KEY_LEFT_SHIFT, DOWN, INPUT_MOVE_DOWN);
    input_handler.bind_key(KEY_W, DOWN, INPUT_MOVE_FORWARD);
    input_handler.bind_key(KEY_S, DOWN, INPUT_MOVE_BACK);
    input_handler.bind_key(KEY_A, DOWN, INPUT_MOVE_LEFT);
    input_handler.bind_key(KEY_D, DOWN, INPUT_MOVE_RIGHT);
    input_handler.bind_key(KEY_E, DOWN, INPUT_ROLL_CW);
    input_handler.bind_key(KEY_Q, DOWN, INPUT_ROLL_CCW);
    input_handler.bind_key(KEY_LEFT_ALT, PRESSED, INPUT_PAUSE);
}

bool Engine::is_active_input(int input) {
    for (size_t i = 0; i < active_inputs.count; ++i) {
        if (active_inputs.commands[i] == input) {
            return true;
        }
    }
    return false;
}

void Engine::process_input() {
    active_inputs = input_handler.handle_input();

    for (size_t i = 0; i < active_inputs.count; ++i) {
        if (active_inputs.commands[i] == INPUT_PAUSE) {
            is_paused_ = !is_paused_;
        }
    }
}


void Engine::update() {
    if (is_paused_) return;

    scene_->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.on_before_update(*this);
    });

    float dt = GetFrameTime();
    accumulator_ += dt;
    while (accumulator_ >= fixed_dt_){
        scene_->for_each_game_object([this](const std::string& s, GameObject& obj){
            obj.on_fixed_update(*this);
        });
        accumulator_ -= fixed_dt_;
    }

    scene_->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.on_update(*this);
    });

    scene_->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.on_after_update(*this);
    });

}

void Engine::render() {
    renderer_->render();
}

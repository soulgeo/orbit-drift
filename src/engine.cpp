#include "engine.hpp"
#include "game_object.hpp"
#include "resource_manager.hpp"
#include <memory>
#include <vector>
#include <string>

struct EngineImpl {
    std::vector<std::string> debug_lines;
};

Engine::Engine() : is_running_(true), is_paused_(false) {
    impl_ = std::make_unique<EngineImpl>();
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

Engine::~Engine() = default;

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

    dt_ = GetFrameTime();
    accumulator_ += dt_;
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
    _write_debug();
    renderer_->render(*this);
}

void Engine::_write_debug() {
    impl_->debug_lines.clear();

    // Gather from Renderer
    Debug& renderer_debug = renderer_->get_debug();
    for (int i = 0; i < renderer_debug.get_line_count(); ++i) {
        impl_->debug_lines.push_back(renderer_debug.get_line(i));
    }

    // Gather from GameObjects
    scene_->for_each_game_object([this](const std::string& s, GameObject& obj){
        Debug& obj_debug = obj.get_debug();
        for (int i = 0; i < obj_debug.get_line_count(); ++i) {
            impl_->debug_lines.push_back(obj_debug.get_line(i));
        }
    });
}

int Engine::get_debug_line_count() const {
    return static_cast<int>(impl_->debug_lines.size());
}

const char* Engine::get_debug_line(int index) const {
    if (index >= 0 && index < static_cast<int>(impl_->debug_lines.size())) {
        return impl_->debug_lines[index].c_str();
    }
    return nullptr;
}

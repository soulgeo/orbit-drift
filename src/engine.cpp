#include "engine.hpp"
#include "game_object.hpp"
#include "resource_manager.hpp"
#include <memory>
#include <vector>
#include <string>
#include "timer.hpp"

struct Engine::Impl {
    std::vector<std::string> debug_lines;
    InputHandler::CommandList active_inputs;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Scene> scene;
    std::unique_ptr<ResourceManager> rsrc_manager;
    bool is_running;
    bool is_paused;
    float dt;
    float fixed_dt = 0.005f;
    float accumulator = 0.0f;
};

Engine::Engine() {
    impl_ = std::make_unique<Impl>();
    impl_->is_running = true;
    impl_->is_paused = false;
    impl_->renderer = std::make_unique<Renderer>();
    impl_->rsrc_manager = std::make_unique<ResourceManager>();
    impl_->scene = std::make_unique<Scene>(impl_->renderer.get(), impl_->rsrc_manager.get());

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
    for (size_t i = 0; i < impl_->active_inputs.count; ++i) {
        if (impl_->active_inputs.commands[i] == input) {
            return true;
        }
    }
    return false;
}

float Engine::get_dt() const {
    return impl_->dt;
}

float Engine::get_fixed_dt() const {
    return impl_->fixed_dt;
}

float Engine::get_interpolation_alpha() const {
    return impl_->accumulator / impl_->fixed_dt;
}

Scene& Engine::get_scene() const {
    return *impl_->scene;
}

void Engine::run() {
    while (impl_->is_running && !WindowShouldClose()) {
        process_input();
        update();
        render();
    }
}

void Engine::process_input() {
    impl_->active_inputs = input_handler.handle_input();

    for (size_t i = 0; i < impl_->active_inputs.count; ++i) {
        if (impl_->active_inputs.commands[i] == INPUT_PAUSE) {
            impl_->is_paused = !impl_->is_paused;
        }
    }
}


void Engine::update() {
    if (impl_->is_paused) return;

    Timer timer("Engine::update");

    impl_->scene->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.on_before_update(*this);
    });

    impl_->dt = GetFrameTime();
    impl_->accumulator += impl_->dt;
    while (impl_->accumulator >= impl_->fixed_dt){
        impl_->scene->for_each_game_object([this](const std::string& s, GameObject& obj){
            obj.on_fixed_update(*this);
        });
        impl_->accumulator -= impl_->fixed_dt;
    }

    impl_->scene->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.on_update(*this);
    });

    impl_->scene->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.on_after_update(*this);
    });

}

void Engine::render() {
    _write_debug();
    impl_->renderer->render(*this);
}

void Engine::_write_debug() {
    impl_->debug_lines.clear();

    // Gather from Renderer
    Debug& renderer_debug = impl_->renderer->get_debug();
    for (int i = 0; i < renderer_debug.get_line_count(); ++i) {
        impl_->debug_lines.push_back(renderer_debug.get_line(i));
    }

    // Gather from GameObjects
    impl_->scene->for_each_game_object([this](const std::string& s, GameObject& obj){
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

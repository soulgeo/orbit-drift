#include "engine.hpp"
#include "game_object.hpp"
#include "resource_manager.hpp"
#include <memory>
#include <string>
#include "timer.hpp"

struct Engine::Impl {
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
    impl_->scene = std::make_unique<Scene>(this, impl_->renderer.get(), impl_->rsrc_manager.get());

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
    start();
    while (impl_->is_running && !WindowShouldClose()) {
        process_input();
        update();
        render();
    }
}

void Engine::start() {
    impl_->scene->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.start();
    });
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
        obj.early_update();
    });

    impl_->dt = GetFrameTime();
    impl_->accumulator += impl_->dt;
    while (impl_->accumulator >= impl_->fixed_dt){
        impl_->scene->for_each_game_object([this](const std::string& s, GameObject& obj){
            obj.fixed_update();
        });
        impl_->accumulator -= impl_->fixed_dt;
    }

    impl_->scene->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.update();
    });

    impl_->scene->for_each_game_object([this](const std::string& s, GameObject& obj){
        obj.late_update();
    });

}

void Engine::render() {
    impl_->renderer->render(*this);
}

#include "sputnik/core/engine.hpp"
#include "sputnik/ecs/game_object.hpp"
#include "sputnik/rendering/renderer.hpp"
#include "sputnik/core/resource_manager.hpp"
#include <memory>
#include <string>
#include "sputnik/core/timer.hpp"
#include "sputnik/physics/physics.hpp"

namespace Sputnik {

    Engine::Engine() : 
        renderer_(Renderer()), physics_(Physics()), rsrc_manager_(ResourceManager())
    {
        is_running_ = true;
        is_paused_ = false;
    }

    Engine::~Engine() = default;

    Renderer& Engine::renderer() {
        return renderer_;
    }

    ResourceManager& Engine::resource_manager() {
        return rsrc_manager_;
    }

    Physics& Engine::physics() {
        return physics_;
    }

    InputHandler& Engine::input_handler() {
        return input_handler_;
    }

    bool Engine::is_paused() const {
        return is_paused_;
    }

    void Engine::toggle_pause() {
        is_paused_ = !is_paused_;
    }

    float Engine::dt() const {
        return dt_;
    }

    float Engine::fixed_dt() const {
        return fixed_dt_;
    }

    float Engine::interpolation_alpha() const {
        return accumulator_ / fixed_dt_;
    }

    Scene& Engine::scene() const {
        return *scene_;
    }

    void Engine::set_scene(std::unique_ptr<Scene> scene) {
        scene_ = std::move(scene);
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
        input_handler_.update();
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

}

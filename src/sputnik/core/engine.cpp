#include "sputnik/core/engine.hpp"
#include "events/event_dispatcher.hpp"
#include "sputnik/ecs/game_object.hpp"
#include "sputnik/rendering/renderer.hpp"
#include "sputnik/core/resource_manager.hpp"
#include <memory>
#include "sputnik/core/timer.hpp"
#include "sputnik/physics/physics.hpp"

namespace Sputnik {

    Engine::Engine() : 
        resource_mgr_(ResourceManager()),
        renderer_(Renderer(&resource_mgr_)), 
        physics_(Physics()), 
        event_dsp_(EventDispatcher())
    {
        is_running_ = true;
        is_paused_ = false;
    }

    Engine::~Engine() = default;

    Renderer& Engine::renderer() {
        return renderer_;
    }

    ResourceManager& Engine::resource_manager() {
        return resource_mgr_;
    }

    Physics& Engine::physics() {
        return physics_;
    }

    InputHandler& Engine::input_handler() {
        return input_handler_;
    }
    
    EventDispatcher& Engine::event_dsp() {
        return event_dsp_;
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
        init();
        while (is_running_ && !WindowShouldClose()) {
            process_input();
            update();
            render();
        }
    }

    void Engine::init() {
        for (size_t i = 0; i < scene_->game_object_count(); ++i) {
            scene_->game_object_at(i)->init();
        }
    }

    void Engine::process_input() {
        input_handler_.update();
    }


    void Engine::update() {
        if (is_paused_) return;

        Timer timer("Engine::update");

        // Early updates
        for (size_t i = 0; i < scene_->game_object_count(); ++i) {
            scene_->game_object_at(i)->early_update();
        }

        // Fixed updates
        dt_ = GetFrameTime();
        accumulator_ += dt_;
        while (accumulator_ >= fixed_dt_){
            for (size_t i = 0; i < scene_->game_object_count(); ++i) {
                scene_->game_object_at(i)->fixed_update();
            }
            physics_.update(this);
            accumulator_ -= fixed_dt_;
        }

        // Updates
        event_dsp_.update();
        for (size_t i = 0; i < scene_->game_object_count(); ++i) {
            scene_->game_object_at(i)->update();
        }

        // Late updates
        for (size_t i = 0; i < scene_->game_object_count(); ++i) {
            scene_->game_object_at(i)->late_update();
        }

    }

    void Engine::render() {
        renderer_.render(*this);
    }

}

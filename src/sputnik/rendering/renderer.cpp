#include "sputnik/rendering/renderer.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include "sputnik/ecs/game_object.hpp"
#include <raylib.h>
#include "sputnik/rendering/camera_component.hpp"
#include "sputnik/rendering/renderable_component.hpp"
#include "sputnik/core/engine.hpp"
#include "sputnik/rendering/debug_component.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else
#define GLSL_VERSION 100
#endif

#include <raymath.h>

namespace Sputnik {

    Renderer::Renderer() : 
        camera_body_(nullptr),
        show_debug_(false)
    {
    }

    Renderer::~Renderer() = default;

    void Renderer::register_renderable(RenderableComponent* renderable) {
        if (renderable->alpha() >= 1.0f) {
            opaque_renderables_.push_back(renderable);
        } else {
            transparent_renderables_.push_back(renderable);
        }
    }

    void Renderer::unregister_renderable(RenderableComponent* renderable) {
        if (renderable->alpha() >= 1.0f) {
            opaque_renderables_.erase(
                std::remove(opaque_renderables_.begin(), opaque_renderables_.end(), renderable), 
                opaque_renderables_.end()
            );
        } else {
            transparent_renderables_.erase(
                std::remove(transparent_renderables_.begin(), transparent_renderables_.end(), renderable), 
                transparent_renderables_.end()
            );
        }
    }

    void Renderer::update_renderable_alpha(RenderableComponent* renderable, float old_alpha, float new_alpha) {
        bool was_opaque = old_alpha >= 1.0f;
        bool is_opaque = new_alpha >= 1.0f;

        if (was_opaque != is_opaque) {
            if (was_opaque) {
                opaque_renderables_.erase(
                    std::remove(opaque_renderables_.begin(), opaque_renderables_.end(), renderable), 
                    opaque_renderables_.end()
                );
                transparent_renderables_.push_back(renderable);
            } else {
                transparent_renderables_.erase(
                    std::remove(transparent_renderables_.begin(), transparent_renderables_.end(), renderable), 
                    transparent_renderables_.end()
                );
                opaque_renderables_.push_back(renderable);
            }
        }
    }

    void Renderer::register_debug(DebugComponent* debug) {
        debugs_.push_back(debug);
    }

    void Renderer::unregister_debug(DebugComponent* debug) {
        debugs_.erase(
            std::remove(debugs_.begin(), debugs_.end(), debug), 
            debugs_.end()
        );
    }

    void Renderer::register_camera(CameraComponent* camera_body) {
        camera_body_ = camera_body;
    }

    void Renderer::set_show_debug(bool show) {
        show_debug_ = show;
    }

    void Renderer::toggle_show_debug() {
        show_debug_ = !show_debug_;
    }

    bool Renderer::is_showing_debug() const {
        return show_debug_;
    }

    void Renderer::render(Engine& engine) {
        if (camera_body_) {
            camera_.position = camera_body_->visual_position();
            camera_.projection = camera_body_->projection();
            camera_.fovy = (float)camera_body_->fovy();
            camera_.target = camera_body_->target();
            camera_.up = camera_body_->camera_up();
        }

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera_);
                draw_3d();
            EndMode3D();
            draw_ui();
        EndDrawing();
    }

    void Renderer::draw_3d() {
        for (auto renderable : opaque_renderables_) {
            renderable->draw();
        }

        Vector3 camera_pos = camera_.position;
        std::sort(transparent_renderables_.begin(), transparent_renderables_.end(), [camera_pos](const RenderableComponent* a, const RenderableComponent* b) {
            float dist_sq_a = Vector3DistanceSqr(a->position(), camera_pos);
            float dist_sq_b = Vector3DistanceSqr(b->position(), camera_pos);
            return dist_sq_a > dist_sq_b;
        });

        for (auto renderable : transparent_renderables_) {
            renderable->draw();
        }
    }

    void Renderer::draw_ui() {
        if (show_debug_) {
            int x = 30;
            int y = 50;
            for (auto i = debugs_.begin(); i != debugs_.end(); i++){
                for (int j = 0; j < (*i)->line_count(); ++j) {
                    DrawText((*i)->line(j), x, y, 20, YELLOW);
                    y += 25;
                }
            }
        }
    }

}

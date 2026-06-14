#include "renderer.hpp"

#include <algorithm>
#include <vector>
#include "game_object.hpp"
#include "raylib.h"
#include "camera_component.hpp"
#include "renderable_component.hpp"
#include "engine.hpp"
#include "debug_component.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else
#define GLSL_VERSION 100
#endif

Renderer::Renderer() : show_debug_(false){
    // Init shaders
    // fog_ = LoadShader(TextFormat("resources/shaders/ambient.vert", GLSL_VERSION),
    //                  TextFormat("resources/shaders/ambient.frag", GLSL_VERSION));
    //
    // for (auto &[name, tuple] : obj_model_refs) {
    //     std::get<0>(tuple).materials[0].shader = fog_;
    // }
}

void Renderer::register_renderable(RenderableComponent* renderable) {
    renderables_.push_back(renderable);
}

void Renderer::unregister_renderable(RenderableComponent* renderable) {
    renderables_.erase(
        std::remove(renderables_.begin(), renderables_.end(), renderable), 
        renderables_.end()
    );
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

Renderer::~Renderer() = default;

void Renderer::register_camera(CameraComponent* camera_body) {
    camera_body_ = camera_body;
}

void Renderer::render(Engine& engine) {
    if (camera_body_) {
        camera_.position = camera_body_->get_visual_position();
        camera_.projection = camera_body_->get_projection();
        camera_.fovy = (float)camera_body_->get_fovy();
        camera_.target = camera_body_->get_target();
        camera_.up = camera_body_->get_camera_up();
    }

    if (engine.is_active_input(INPUT_DEBUG)) {
        show_debug_ = !show_debug_;
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
    //
    // int distLoc = GetShaderLocation(fog_, "viewPos");
    // SetShaderValue(fog_, distLoc, &camera_.position, SHADER_UNIFORM_VEC3);
    // SetShaderValue(fog_, fog_.locs[SHADER_LOC_VECTOR_VIEW], &camera_.position,
    //                SHADER_UNIFORM_VEC3);

    for (auto& renderable : renderables_) {
        renderable->draw();
    }
}

void Renderer::draw_ui() {
    if (show_debug_) {
        int x = 30;
        int y = 50;
        for (auto i = debugs_.begin(); i != debugs_.end(); i++){
            for (int j = 0; j < (*i)->get_line_count(); ++j) {
                DrawText((*i)->get_line(j), x, y, 20, YELLOW);
                y += 25;
            }
        }
    }
}

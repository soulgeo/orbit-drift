#include "renderer.hpp"

#include <algorithm>
#include <memory>
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


struct Renderer::Impl {
    std::vector<RenderableComponent*> renderables;
    std::vector<DebugComponent*> debugs;
    Camera camera = {0};
    CameraComponent* camera_body;
    Shader fog;
    float dt;
};

Renderer::Renderer() {
    impl_ = std::make_unique<Impl>();

    // Init shaders
    // impl_->fog = LoadShader(TextFormat("resources/shaders/ambient.vert", GLSL_VERSION),
    //                  TextFormat("resources/shaders/ambient.frag", GLSL_VERSION));
    //
    // for (auto &[name, tuple] : impl_->obj_model_refs) {
    //     std::get<0>(tuple).materials[0].shader = impl_->fog;
    // }
}

void Renderer::register_renderable(RenderableComponent* renderable) {
    impl_->renderables.push_back(renderable);
}

void Renderer::unregister_renderable(RenderableComponent* renderable) {
    impl_->renderables.erase(
        std::remove(impl_->renderables.begin(), impl_->renderables.end(), renderable), 
        impl_->renderables.end()
    );
}

void Renderer::register_debug(DebugComponent* debug) {
    impl_->debugs.push_back(debug);
}

void Renderer::unregister_debug(DebugComponent* debug) {
    impl_->debugs.erase(
        std::remove(impl_->debugs.begin(), impl_->debugs.end(), debug), 
        impl_->debugs.end()
    );
}

Renderer::~Renderer() = default;

void Renderer::register_camera(CameraComponent* camera_body) {
    impl_->camera_body = camera_body;
}

void Renderer::render(Engine& engine) {
    if (impl_->camera_body) {
        impl_->camera.position = impl_->camera_body->get_position();
        impl_->camera.projection = impl_->camera_body->get_projection();
        impl_->camera.fovy = (float)impl_->camera_body->get_fovy();
        impl_->camera.target = impl_->camera_body->get_target();
        impl_->camera.up = impl_->camera_body->get_camera_up();
    }

    BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(impl_->camera);
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

    for (auto& renderable : impl_->renderables) {
        renderable->draw();
    }
}

void Renderer::draw_ui() {
    int x = 30;
    int y = 50;
    for (auto i = impl_->debugs.begin(); i != impl_->debugs.end(); i++){
        for (int j = 0; j < (*i)->get_line_count(); ++j) {
            DrawText((*i)->get_line(j), x, y, 20, YELLOW);
            y += 25;
        }
    }
}

// struct ShakeSetting {
//     float min;
//     float max;
//     float speed;
//     float maxFrames;
// };
//
// struct ShakeState {
//     float value;
//     int mult = 1;
//     int accum = 0;
// };
//
// struct Vector3ShakeState {
//     Vector3 value;
//     Vector3 mult = {1, 1, 1};
//     int accum = 0;
// };
//
// void _shake(ShakeState* stateRef, ShakeSetting s) {
//     auto eval = [stateRef, s]() {
//         return stateRef->value + s.speed * stateRef->mult * GetFrameTime();
//     };
//
//     float targetValue = eval();
//     if (targetValue > s.max || targetValue < s.min) {
//         stateRef->mult *= -1;
//         targetValue = eval();
//
//     } else if (stateRef->accum == s.maxFrames) {
//         auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());
//         bool flip = gen();
//
//         if (flip) {
//             stateRef->mult *= -1;
//             targetValue = eval();
//         }
//     }
//     stateRef->value = targetValue;
// }

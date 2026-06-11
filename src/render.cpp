#include "render.hpp"

#include <algorithm>
#include <memory>
#include <vector>
#include "game_object.hpp"
#include "raylib.h"
#include "camera.hpp"
#include "renderable.hpp"
#include "engine.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else
#define GLSL_VERSION 100
#endif


struct Renderer::Impl {
    std::vector<Renderable*> renderables;
    Camera camera = {0};
    CameraBody* camera_body = nullptr;
    Shader fog;
    Debug debug;
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

void Renderer::add_renderable(Renderable* renderable) {
    impl_->renderables.push_back(renderable);
}

void Renderer::remove_renderable(Renderable* renderable) {
    impl_->renderables.erase(
        std::remove(impl_->renderables.begin(), impl_->renderables.end(), renderable), 
        impl_->renderables.end()
    );
}

Renderer::~Renderer() = default;

void Renderer::set_camera_body(CameraBody* camera_body) {
    impl_->camera_body = camera_body;
}

Debug& Renderer::get_debug() {
    return impl_->debug;
}

void Renderer::render(Engine& engine) {
    impl_->debug.clean();
    impl_->debug.writeln("--- CAMERA ---");
    impl_->debug.writeln(TextFormat("Pos: %.2f, %.2f, %.2f", impl_->camera.position.x, impl_->camera.position.y, impl_->camera.position.z));
    impl_->debug.writeln(TextFormat("Target: %.2f, %.2f, %.2f", impl_->camera.target.x, impl_->camera.target.y, impl_->camera.target.z));

    if (impl_->camera_body) {
        impl_->camera.position = impl_->camera_body->get_visual_position();
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
        draw_ui(engine);
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

void Renderer::draw_ui(Engine& engine) {
    int x = 30;
    int y = 50;
    for (int i = 0; i < engine.get_debug_line_count(); ++i) {
        DrawText(engine.get_debug_line(i), x, y, 20, YELLOW);
        y += 25;
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

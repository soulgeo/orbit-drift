#include "render.hpp"

#include <algorithm>
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


struct RendererImpl {
    std::vector<Renderable*> renderables;
};

Renderer::Renderer() {
    impl_ = new RendererImpl;

    // Init shaders
    // fog_ = LoadShader(TextFormat("resources/shaders/ambient.vert", GLSL_VERSION),
    //                  TextFormat("resources/shaders/ambient.frag", GLSL_VERSION));
    //
    // for (auto &[name, tuple] : impl_->obj_model_refs) {
    //     std::get<0>(tuple).materials[0].shader = fog_;
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

Renderer::~Renderer() {
    delete impl_;
}

void Renderer::render(Engine& engine) {
    debug_.clean();
    debug_.writeln("--- CAMERA ---");
    debug_.writeln(TextFormat("Pos: %.2f, %.2f, %.2f", camera_.position.x, camera_.position.y, camera_.position.z));
    debug_.writeln(TextFormat("Target: %.2f, %.2f, %.2f", camera_.target.x, camera_.target.y, camera_.target.z));

    if (camera_body_) {
        camera_.position = camera_body_->get_visual_position();
        camera_.projection = camera_body_->get_projection();
        camera_.fovy = (float)camera_body_->get_fovy();
        camera_.target = camera_body_->get_target();
        camera_.up = camera_body_->get_camera_up();
    }

    BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera_);
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

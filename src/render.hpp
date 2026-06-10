#ifndef RENDER_HPP
#define RENDER_HPP

#include "raylib.h"
#include "debug.hpp"

class Renderable;
class CameraBody;
class Engine;

struct RendererImpl;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void add_renderable(Renderable* renderable);
    void remove_renderable(Renderable* renderable);

    void set_camera_body(CameraBody* camera_body){
        camera_body_ = camera_body;
    };

    void render(Engine& engine);
    void draw_3d();
    void draw_ui(Engine& engine);

    Debug& get_debug() { return debug_; }

private:
    RendererImpl* impl_;
    Camera camera_ = {0};
    CameraBody* camera_body_ = nullptr;
    Shader fog_;
    Debug debug_;

    float dt_;
};

#endif // !RENDER_HPP

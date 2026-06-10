#ifndef RENDER_HPP
#define RENDER_HPP

#include "raylib.h"

class Renderable;
class CameraBody;

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

    void render();
    void draw_3d();
    void draw_ui();

private:
    RendererImpl* impl_;
    Camera camera_ = {0};
    CameraBody* camera_body_ = nullptr;
    Shader fog_;

    float dt_;
};


#endif // !RENDER_HPP

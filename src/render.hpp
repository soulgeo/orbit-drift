#ifndef RENDER_HPP
#define RENDER_HPP

#include "debug.hpp"
#include <memory>

class RenderableComponent;
class CameraBody;
class Engine;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void register_renderable(RenderableComponent* renderable);
    void unregister_renderable(RenderableComponent* renderable);

    void set_camera_body(CameraBody* camera_body);

    void render(Engine& engine);
    void draw_3d();
    void draw_ui(Engine& engine);

    Debug& get_debug();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif // !RENDER_HPP

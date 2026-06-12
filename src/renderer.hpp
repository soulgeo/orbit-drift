#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

class RenderableComponent;
class DebugComponent;
class CameraComponent;
class Engine;

class Renderer {
    struct Impl;
    std::unique_ptr<Impl> impl_;

public:
    Renderer();
    ~Renderer();

    void register_renderable(RenderableComponent* renderable);
    void unregister_renderable(RenderableComponent* renderable);

    void register_debug(DebugComponent* debug);
    void unregister_debug(DebugComponent* debug);

    void register_camera(CameraComponent* camera_comp);

    void render(Engine& engine);
    void draw_3d();
    void draw_ui();
};

#endif // !RENDERER_HPP

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "raylib.h"
#include <vector>

class RenderableComponent;
class DebugComponent;
class CameraComponent;
class Engine;

class Renderer {
    std::vector<RenderableComponent*> renderables_;
    std::vector<DebugComponent*> debugs_;
    Camera camera_ = {0};
    CameraComponent* camera_body_;
    Shader fog_;
    float dt_;
    bool show_debug_;

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

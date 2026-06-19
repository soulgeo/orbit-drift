#pragma once

#include "core/resource_manager.hpp"
#include <raylib.h>
#include <vector>

namespace Sputnik {

    class RenderableComponent;
    class DebugComponent;
    class CameraComponent;
    class Engine;

    class Renderer {
        std::vector<RenderableComponent*> opaque_renderables_;
        std::vector<RenderableComponent*> transparent_renderables_;
        std::vector<DebugComponent*> debugs_;
        Camera camera_ = {0};
        CameraComponent* camera_body_;
        Shader shader_;
        float dt_;
        bool show_debug_;

    public:
        Renderer(ResourceManager* resource_mgr);
        ~Renderer();

        void register_renderable(RenderableComponent* renderable);
        void unregister_renderable(RenderableComponent* renderable);
        void update_renderable_alpha(RenderableComponent* renderable, float old_alpha, float new_alpha);

        void register_debug(DebugComponent* debug);
        void unregister_debug(DebugComponent* debug);

        void register_camera(CameraComponent* camera_body);

        void set_show_debug(bool show);
        void toggle_show_debug();
        bool is_showing_debug() const;

        void render(Engine& engine);

        void draw_3d();
        void draw_ui();
    };

}

#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "sputnik/ecs/component.hpp"
#include <raylib.h>
#include "sputnik/rendering/renderer.hpp"
#include "sputnik/ecs/transform_component.hpp"

namespace Sputnik {

    class RenderableComponent : public Component {
        Color color_;
        float alpha_;
        Model model_;
        bool draw_wires_;
        Matrix initial_transform_;
        TransformComponent* transform_;
        DebugComponent* debug_;
        Renderer* renderer_;

    public:
        RenderableComponent(GameObject* owner, Renderer* renderer, Model& model);
        ~RenderableComponent();

        void start() override;
        void update() override;
        void draw();

        Vector3 position() const;
        float alpha() const;
        bool draw_wires() const;

        void set_color(Color color);
        void set_alpha(float alpha);
        void set_draw_wires(bool draw_wires);
        void set_initial_transform(Matrix transform);
    };

}

#endif // !RENDERABLE_HPP

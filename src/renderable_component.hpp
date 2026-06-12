#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "component.hpp"
#include "raylib.h"
#include "renderer.hpp"
#include "transform_component.hpp"

class RenderableComponent : public Component {
    Color color_;
    Model model_;
    Matrix initial_transform_;
    TransformComponent* cached_transform;
public:
    RenderableComponent(GameObject* owner, Renderer* renderer, Model& model);
    ~RenderableComponent();

    void start() override;
    void update() override;
    void draw();

    Vector3 get_position();

    void set_color(Color color);
    void set_initial_transform(Matrix transform);
};

#endif // !RENDERABLE_HPP

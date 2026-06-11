#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "component.hpp"
#include "raylib.h"
#include "render.hpp"

class RenderableComponent : public Component {
public:
    RenderableComponent(GameObject* owner, Renderer* renderer, Model& model);
    ~RenderableComponent();
    void update() override;
    void draw();
    void set_color(Color color){
        color_ = color;
    }
    void set_initial_transform(Matrix transform){
        initial_transform_ = transform;
    }

    Vector3 get_position() {
        return (Vector3){
            model_.transform.m12, 
            model_.transform.m13, 
            model_.transform.m14
        };
    }

protected:
    Color color_;
    Model model_;
    Matrix initial_transform_;
};

#endif // !RENDERABLE_HPP

#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "raylib.h"
#include "render.hpp"

class GameObject;

class Renderable {
public:
    Renderable(GameObject* owner, Renderer* renderer, Model& model);
    ~Renderable();
    void update(Matrix transform);
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
    GameObject* owner_;
    Color color_;
    Model model_;
    Matrix initial_transform_;
};

#endif // !RENDERABLE_HPP

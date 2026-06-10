#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "raylib.h"
#include "render.hpp"

class GameObject;

class Renderable {
public:
    Renderable(GameObject* owner, Renderer* renderer, Model& model);
    ~Renderable();
    void update();
    void draw();
    void set_color(Color& color){
        color_ = color;
    }

protected:
    GameObject* owner_;
    Color color_;
    Model model_;
};

#endif // !RENDERABLE_HPP

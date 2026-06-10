#include "renderable.hpp"
#include "game_object.hpp"

Renderable::Renderable(GameObject* owner, Renderer* renderer, Model& model) 
    : model_(model), 
      owner_(owner) 
{
    renderer->add_renderable(this);
}
 
Renderable::~Renderable() {
    UnloadModel(model_);
}

void Renderable::update() {
    if (owner_ == nullptr) {
        return;
    }
    model_.transform = owner_->get_transform();
}

void Renderable::draw() {
    DrawModel(model_, (Vector3) {0.0f, 0.0f, 0.0f}, 1.0f, color_);
}

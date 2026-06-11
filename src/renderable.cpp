#include "renderable.hpp"
#include "game_object.hpp"
#include "raymath.h"

RenderableComponent::RenderableComponent(GameObject* owner, Renderer* renderer, Model& model) 
    : Component(owner), model_(model)
{
    renderer->register_renderable(this);
}
 
RenderableComponent::~RenderableComponent() {
    UnloadModel(model_);
}

void RenderableComponent::update() {
    if (owner_ == nullptr) { return; }
    model_.transform = initial_transform_ * owner_->get_visual_transform();
}

void RenderableComponent::draw() {
    DrawModel(model_, (Vector3) {0.0f, 0.0f, 0.0f}, 1.0f, color_);
}

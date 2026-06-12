#include "renderable_component.hpp"
#include "game_object.hpp"
#include "raymath.h"
#include "transform_component.hpp"

RenderableComponent::RenderableComponent(GameObject* owner, Renderer* renderer, Model& model) 
    : Component(owner), model_(model)
{
    renderer->register_renderable(this);
}
 
RenderableComponent::~RenderableComponent() {
    UnloadModel(model_);
}

Vector3 RenderableComponent::get_position() {
    return (Vector3){
        model_.transform.m12, 
        model_.transform.m13, 
        model_.transform.m14
    };
}

void RenderableComponent::set_color(Color color){
    color_ = color; 
}

void RenderableComponent::set_initial_transform(Matrix transform){
    initial_transform_ = transform;
}

void RenderableComponent::start() {
    cached_transform = owner_->get_component<TransformComponent>();
}

void RenderableComponent::update() {
    if (owner_ == nullptr) { return; }
    model_.transform = 
        initial_transform_ * 
        cached_transform->get_visual_transform();
}

void RenderableComponent::draw() {
    DrawModel(model_, (Vector3) {0.0f, 0.0f, 0.0f}, 1.0f, color_);
}

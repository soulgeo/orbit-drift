#include "renderable_component.hpp"
#include "game_object.hpp"
#include "raylib.h"
#include "raymath.h"
#include "transform_component.hpp"
#include "debug_component.hpp"

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
    transform_ = owner_->get_component<TransformComponent>();
    debug_ = owner_->get_component<DebugComponent>();
}

void RenderableComponent::update() {
    if (owner_ == nullptr) { return; }
    model_.transform = 
        initial_transform_ * 
        transform_->get_visual_transform();

    Vector3 position = transform_->get_position();
    Vector3 up = transform_->get_up();
    if (debug_) {
        debug_->writeln(TextFormat("--- MODEL ---"));
        debug_->writeln(TextFormat("Position: %.2f, %.2f, %.2f", 
                                   position.x, position.y, position.z));
        debug_->writeln(TextFormat("Up Vector: %.2f, %.2f, %.2f", 
                                   up.x, up.y, up.z));
    }
}

void RenderableComponent::draw() {
    DrawModel(model_, (Vector3) {0.0f, 0.0f, 0.0f}, 1.0f, color_);
}

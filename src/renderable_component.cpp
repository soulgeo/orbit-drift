#include "renderable_component.hpp"
#include "game_object.hpp"
#include "raylib.h"
#include "raymath.h"
#include "transform_component.hpp"
#include "debug_component.hpp"

RenderableComponent::RenderableComponent(GameObject* owner, Renderer* renderer, Model& model) : 
    Component(owner), model_(model), color_(RAYWHITE), alpha_(1.0f), renderer_(renderer),
    draw_wires_(false)
{
    renderer_->register_renderable(this);
}
 
RenderableComponent::~RenderableComponent() {
    renderer_->unregister_renderable(this);
    UnloadModel(model_);
}

Vector3 RenderableComponent::get_position() const {
    return (Vector3){
        model_.transform.m12, 
        model_.transform.m13, 
        model_.transform.m14
    };
}

float RenderableComponent::alpha() const {
    return alpha_;
}

bool RenderableComponent::draw_wires() const {
    return draw_wires_;
}

void RenderableComponent::set_color(Color color){
    color_ = color; 
}

void RenderableComponent::set_alpha(float alpha) {
    float old_alpha = alpha_;
    alpha_ = alpha;
    renderer_->update_renderable_alpha(this, old_alpha, alpha_);
}

void RenderableComponent::set_draw_wires(bool draw_wires) {
    draw_wires_ = draw_wires;
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
    // if (debug_) {
    //     debug_->writeln(TextFormat("--- MODEL ---"));
    //     debug_->writeln(TextFormat("Position: %.2f, %.2f, %.2f", 
    //                                position.x, position.y, position.z));
    //     debug_->writeln(TextFormat("Up Vector: %.2f, %.2f, %.2f", 
    //                                up.x, up.y, up.z));
    // }
}

void RenderableComponent::draw() {
    DrawModel(model_, (Vector3) {0.0f, 0.0f, 0.0f}, 1.0f, ColorAlpha(color_, alpha_));
    if (draw_wires_) {
        DrawModelWires(
            model_, (Vector3) {0.0f, 0.0f, 0.0f}, 1.0f, 
            ColorTint(ColorAlpha(color_, alpha_), GRAY)
        );
    }
}

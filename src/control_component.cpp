#include "control_component.hpp"
#include "game_object.hpp"
#include "transform_component.hpp"

ControlComponent::ControlComponent(GameObject* owner) : Component(owner) {
    x_amp_ = 200.0f;
    y_amp_ = 200.0f;
    z_amp_ = 1000.0f;
    roll_amp_ = 5.0f;
    pan_amp_ = 0.004f;
}

ControlComponent::~ControlComponent() = default;

void ControlComponent::start() {
    transform_ = owner_->get_component<TransformComponent>();
    physics_ = owner_->get_component<PhysicsComponent>();
}

void ControlComponent::early_update() {
    Engine* engine = owner_->get_engine();
    x_axis_ = 
        engine->is_active_input(INPUT_MOVE_RIGHT) - 
        engine->is_active_input(INPUT_MOVE_LEFT);
    y_axis_ = 
        engine->is_active_input(INPUT_MOVE_UP) - 
        engine->is_active_input(INPUT_MOVE_DOWN);
    z_axis_ =
        engine->is_active_input(INPUT_MOVE_FORWARD) - 
        engine->is_active_input(INPUT_MOVE_BACK);
    roll_axis_ = 
        engine->is_active_input(INPUT_ROLL_CCW) - 
        engine->is_active_input(INPUT_ROLL_CW);

    Vector2 mouse_position = GetMousePosition();
    pan_ = {mouse_position.x - 960, mouse_position.y - 540};
}

void ControlComponent::fixed_update() {
    Engine* engine = owner_->get_engine();
    float fixed_dt = engine->get_fixed_dt();

    float local_yaw = pan_.x * -pan_amp_ * fixed_dt;
    float local_pitch = pan_.y * -pan_amp_ * fixed_dt;

    transform_->rotate_pitch(local_pitch);
    transform_->rotate_yaw(local_yaw);

    Vector3 force = { x_axis_ * x_amp_, y_axis_ * y_amp_, -z_axis_ * z_amp_ };

    if (physics_){
        physics_->apply_force_local(force);
    }
}

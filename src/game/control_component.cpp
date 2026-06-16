#include "control_component.hpp"
#include "sputnik/ecs/game_object.hpp"
#include "sputnik/ecs/transform_component.hpp"
#include "sputnik/core/engine.hpp"
#include "game_input.hpp"

using namespace Sputnik;

ControlComponent::ControlComponent(GameObject* owner) : Component(owner) {
    x_amp_ = 400.0f;
    y_amp_ = 400.0f;
    z_amp_ = 1600.0f;
    roll_amp_ = 5.0f;
    pan_amp_ = 0.004f;
}

ControlComponent::~ControlComponent() = default;

void ControlComponent::start() {
    transform_ = owner_->component<TransformComponent>();
    physics_ = owner_->component<PhysicsComponent>();
}

void ControlComponent::early_update() {
    Engine* engine = owner_->engine();
    auto& input = engine->input_handler();

    x_axis_ = 
        input.is_action_active(INPUT_MOVE_RIGHT) - 
        input.is_action_active(INPUT_MOVE_LEFT);
    y_axis_ = 
        input.is_action_active(INPUT_MOVE_UP) - 
        input.is_action_active(INPUT_MOVE_DOWN);
    z_axis_ =
        input.is_action_active(INPUT_MOVE_FORWARD) - 
        input.is_action_active(INPUT_MOVE_BACK);
    roll_axis_ = 
        input.is_action_active(INPUT_ROLL_CCW) - 
        input.is_action_active(INPUT_ROLL_CW);

    Vector2 mouse_position = GetMousePosition();
    pan_ = {mouse_position.x - 960, mouse_position.y - 540};
}

void ControlComponent::fixed_update() {
    Engine* engine = owner_->engine();
    float fixed_dt = engine->fixed_dt();

    float local_yaw = pan_.x * -pan_amp_ * fixed_dt;
    float local_pitch = pan_.y * -pan_amp_ * fixed_dt;

    transform_->rotate_pitch(local_pitch);
    transform_->rotate_yaw(local_yaw);

    Vector3 force = { x_axis_ * x_amp_, y_axis_ * y_amp_, -z_axis_ * z_amp_ };

    if (physics_){
        physics_->apply_force_local(force);
    }
}

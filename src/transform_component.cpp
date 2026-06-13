#include "transform_component.hpp"
#include "game_object.hpp"
#include "component.hpp"
#include "raymath.h"

TransformComponent::TransformComponent(GameObject* owner, Engine* engine) : Component(owner) {
    transform_ = MatrixIdentity();
    previous_transform_ = MatrixIdentity();
    visual_transform_ = MatrixIdentity();
    engine_ = engine;
}

TransformComponent::~TransformComponent() = default;

Matrix TransformComponent::get_transform() const {
    return transform_;
}

Matrix TransformComponent::get_visual_transform() const {
    return visual_transform_;
}

//================================================================================== 
// Global Movement

void TransformComponent::set_position_x(float x) {
    transform_.m12 = x;
}
void TransformComponent::set_position_y(float y) {
    transform_.m13 = y;
}
void TransformComponent::set_position_z(float x) {
    transform_.m14 = x;
}
void TransformComponent::set_position(float x, float y, float z) {
    set_position_x(x);
    set_position_y(y);
    set_position_z(z);
}
void TransformComponent::set_position(Vector3 position) {
    set_position_x(position.x);
    set_position_y(position.y);
    set_position_z(position.z);
}
void TransformComponent::move_global_x(float delta_x) { 
    transform_.m12 += delta_x; 
}
void TransformComponent::move_global_y(float delta_y) { 
    transform_.m13 += delta_y; 
}
void TransformComponent::move_global_z(float delta_z) { 
    transform_.m14 += delta_z; 
}

void TransformComponent::move_global(float delta_x, float delta_y, float delta_z) {
    move_global_x(delta_x); move_global_y(delta_y); move_global_z(delta_z);
}

void TransformComponent::move_global(Vector3 delta) {
    move_global_x(delta.x); move_global_y(delta.y); move_global_z(delta.z); 
}

void TransformComponent::move_global_vel(Vector3 velocity, float dt) {
    Vector3 displacement = velocity * dt;
    move_global(displacement);
}

//================================================================================== 
// Local Movement

Vector3 TransformComponent::get_position() const { 
    return (Vector3){ transform_.m12, transform_.m13, transform_.m14 }; 
}

Vector3 TransformComponent::get_previous_position() const { 
    return (Vector3){ previous_transform_.m12, previous_transform_.m13, previous_transform_.m14 }; 
}

Vector3 TransformComponent::get_visual_position() const { 
    return (Vector3){ visual_transform_.m12, visual_transform_.m13, visual_transform_.m14 }; 
}

Vector3 TransformComponent::get_right() const { 
    return (Vector3){ transform_.m0,  transform_.m1,  transform_.m2 }; 
}
Vector3 TransformComponent::get_up() const { 
    return (Vector3){ transform_.m4,  transform_.m5,  transform_.m6 }; 
}
Vector3 TransformComponent::get_forward() const { 
    return (Vector3){ -transform_.m8, -transform_.m9, -transform_.m10 }; 
}

void TransformComponent::move_local_right(float distance) {
    Matrix moveMat = MatrixTranslate(distance, 0.0f, 0.0f);
    transform_ = moveMat * transform_;
}
void TransformComponent::move_local_up(float distance) {
    Matrix moveMat = MatrixTranslate(0.0f, distance, 0.0f);
    transform_ = moveMat * transform_;
}
void TransformComponent::move_local_forward(float distance) {
    Matrix moveMat = MatrixTranslate(0.0f, 0.0f, -distance);
    transform_ = moveMat * transform_;
}

//================================================================================== 
// Rotation
void TransformComponent::rotate_pitch(float angle_rad) {
    transform_ = MatrixRotateX(angle_rad) * transform_;
}
void TransformComponent::rotate_yaw(float angle_rad) {
    transform_ = MatrixRotateY(angle_rad) * transform_;
}
void TransformComponent::rotate_roll(float angle_rad) {
    transform_ = MatrixRotateZ(angle_rad) * transform_;
}

void TransformComponent::rotate(float delta_pitch, float delta_yaw, float delta_roll) {
    Matrix p_mat = MatrixRotateX(delta_pitch);
    Matrix y_mat = MatrixRotateY(delta_yaw);
    Matrix r_mat = MatrixRotateZ(delta_roll);

    Matrix frame_rotation = p_mat * y_mat * r_mat;

    transform_ = frame_rotation * transform_;
}

// Updates
void TransformComponent::fixed_update() {
    previous_transform_ = transform_;
}

void TransformComponent::update() {
    float alpha = engine_->get_interpolation_alpha();
    
    Vector3 old_pos = { previous_transform_.m12, previous_transform_.m13, previous_transform_.m14 };
    Vector3 curr_pos = { transform_.m12, transform_.m13, transform_.m14 };
    Vector3 interp_pos = Vector3Lerp(old_pos, curr_pos, alpha);

    Quaternion old_rot = QuaternionFromMatrix(previous_transform_);
    Quaternion curr_rot = QuaternionFromMatrix(transform_);
    Quaternion interp_rot = QuaternionSlerp(old_rot, curr_rot, alpha);

    // TODO: Handle scale if needed. For now assuming uniform scale 1.0
    visual_transform_ = QuaternionToMatrix(interp_rot);
    visual_transform_.m12 = interp_pos.x;
    visual_transform_.m13 = interp_pos.y;
    visual_transform_.m14 = interp_pos.z;
}

#include "game_object.hpp"
#include "raymath.h"
#include "engine.hpp"
#include <memory>

void GameObject::add_renderable(std::unique_ptr<Renderable> rend) {
    renderable_ = std::move(rend);
}

//================================================================================== 
// Global Movement

void GameObject::set_position_x(float x) {
    this->transform_.m12 = x; 
}
void GameObject::set_position_y(float y) {
    this->transform_.m13 = y; 
}
void GameObject::set_position_z(float z) {
    this->transform_.m14 = z; 
}
void GameObject::set_position(float x, float y, float z) {
    set_position_x(x); 
    set_position_y(y); 
    set_position_z(z);
}
void GameObject::set_position(Vector3 position) {
    set_position_x(position.x); 
    set_position_y(position.y); 
    set_position_z(position.z);
}
void GameObject::move_global_x(float delta_x) { this->transform_.m12 += delta_x; }
void GameObject::move_global_y(float delta_y) { this->transform_.m13 += delta_y; }
void GameObject::move_global_z(float delta_z) { this->transform_.m14 += delta_z; }

void GameObject::move_global(float delta_x, float delta_y, float delta_z) {
    move_global_x(delta_x); move_global_y(delta_y); move_global_z(delta_z); 
}

void GameObject::move_global(Vector3 delta) {
    move_global_x(delta.x); move_global_y(delta.y); move_global_z(delta.z); 
}

void GameObject::move_global_vel(Vector3 velocity) {
    Vector3 displacement = GetFrameTime() * velocity;
    move_global(displacement);
}

//================================================================================== 
// Local Movement

Vector3 GameObject::get_position() const { 
    return (Vector3){ transform_.m12, transform_.m13, transform_.m14 }; 
}

Vector3 GameObject::get_visual_position() const { 
    return (Vector3){ visual_transform_.m12, visual_transform_.m13, visual_transform_.m14 }; 
}

Vector3 GameObject::get_right() const { 
    return (Vector3){ transform_.m0,  transform_.m1,  transform_.m2 }; 
}
Vector3 GameObject::get_up() const { 
    return (Vector3){ transform_.m4,  transform_.m5,  transform_.m6 }; 
}
Vector3 GameObject::get_forward() const { 
    return (Vector3){ -transform_.m8, -transform_.m9, -transform_.m10 }; 
}

void GameObject::move_local_right(float distance) {
    Matrix moveMat = MatrixTranslate(distance, 0.0f, 0.0f);
    this->transform_ = moveMat * this->transform_;
}
void GameObject::move_local_up(float distance) {
    Matrix moveMat = MatrixTranslate(0.0f, distance, 0.0f);
    this->transform_ = moveMat * this->transform_;
}
void GameObject::move_local_forward(float distance) {
    Matrix moveMat = MatrixTranslate(0.0f, 0.0f, -distance);
    this->transform_ = moveMat * this->transform_;
}

//================================================================================== 
// Rotation
void GameObject::rotate_pitch(float angle_rad) {
    this->transform_ = MatrixRotateX(angle_rad) * this->transform_;
}
void GameObject::rotate_yaw(float angle_rad) {
    this->transform_ = MatrixRotateY(angle_rad) * this->transform_;
}
void GameObject::rotate_roll(float angle_rad) {
    this->transform_ = MatrixRotateZ(angle_rad) * this->transform_;
}

void GameObject::rotate(float delta_pitch, float delta_yaw, float delta_roll) {
    Matrix p_mat = MatrixRotateX(delta_pitch);
    Matrix y_mat = MatrixRotateY(delta_yaw);
    Matrix r_mat = MatrixRotateZ(delta_roll);

    Matrix frame_rotation = p_mat * y_mat * r_mat;

    this->transform_ = frame_rotation * this->transform_;
}

//================================================================================== 
// Frame by frame behavior
void GameObject::on_update(Engine& engine){
    update(engine);
    
    float alpha = engine.get_interpolation_alpha();
    
    // Decompose transforms for proper interpolation
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

    if (renderable_) {
        renderable_->update(visual_transform_);
    }
}

void GameObject::on_before_update(Engine& engine){
    before_update(engine);
}

void GameObject::on_after_update(Engine& engine){
    after_update(engine);
}

void GameObject::on_fixed_update(Engine& engine) {
    previous_transform_ = transform_;
    fixed_update(engine);
}

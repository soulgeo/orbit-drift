#include "game_object.hpp"
#include "raymath.h"
#include "engine.hpp"
#include <memory>

struct GameObject::Impl {
    Matrix transform = MatrixIdentity();
    Matrix previous_transform = MatrixIdentity();
    Matrix visual_transform = MatrixIdentity();
    std::unique_ptr<Component> component;
    Debug debug;
};

GameObject::GameObject() : impl_(std::make_unique<Impl>()) {}

GameObject::~GameObject() = default;

void GameObject::add_component(std::unique_ptr<Component> comp) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    impl_->component = std::move(comp);
}

Matrix GameObject::get_transform() const {
    if (!impl_) const_cast<GameObject*>(this)->impl_ = std::make_unique<Impl>();
    return impl_->transform;
}

Matrix GameObject::get_visual_transform() const {
    if (!impl_) const_cast<GameObject*>(this)->impl_ = std::make_unique<Impl>();
    return impl_->visual_transform;
}

Debug& GameObject::get_debug() {
    if (!impl_) impl_ = std::make_unique<Impl>();
    return impl_->debug;
}

//================================================================================== 
// Global Movement

void GameObject::set_position_x(float x) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    impl_->transform.m12 = x;
}
void GameObject::set_position_y(float y) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    impl_->transform.m13 = y;
}
void GameObject::set_position_z(float x) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    impl_->transform.m14 = x;
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
void GameObject::move_global_x(float delta_x) { 
    if (!impl_) impl_ = std::make_unique<Impl>();
    impl_->transform.m12 += delta_x; 
}
void GameObject::move_global_y(float delta_y) { 
    if (!impl_) impl_ = std::make_unique<Impl>();
    impl_->transform.m13 += delta_y; 
}
void GameObject::move_global_z(float delta_z) { 
    if (!impl_) impl_ = std::make_unique<Impl>();
    impl_->transform.m14 += delta_z; 
}

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
    if (!impl_) const_cast<GameObject*>(this)->impl_ = std::make_unique<Impl>();
    return (Vector3){ impl_->transform.m12, impl_->transform.m13, impl_->transform.m14 }; 
}

Vector3 GameObject::get_visual_position() const { 
    if (!impl_) const_cast<GameObject*>(this)->impl_ = std::make_unique<Impl>();
    return (Vector3){ impl_->visual_transform.m12, impl_->visual_transform.m13, impl_->visual_transform.m14 }; 
}

Vector3 GameObject::get_right() const { 
    if (!impl_) const_cast<GameObject*>(this)->impl_ = std::make_unique<Impl>();
    return (Vector3){ impl_->transform.m0,  impl_->transform.m1,  impl_->transform.m2 }; 
}
Vector3 GameObject::get_up() const { 
    if (!impl_) const_cast<GameObject*>(this)->impl_ = std::make_unique<Impl>();
    return (Vector3){ impl_->transform.m4,  impl_->transform.m5,  impl_->transform.m6 }; 
}
Vector3 GameObject::get_forward() const { 
    if (!impl_) const_cast<GameObject*>(this)->impl_ = std::make_unique<Impl>();
    return (Vector3){ -impl_->transform.m8, -impl_->transform.m9, -impl_->transform.m10 }; 
}

void GameObject::move_local_right(float distance) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    Matrix moveMat = MatrixTranslate(distance, 0.0f, 0.0f);
    impl_->transform = moveMat * impl_->transform;
}
void GameObject::move_local_up(float distance) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    Matrix moveMat = MatrixTranslate(0.0f, distance, 0.0f);
    impl_->transform = moveMat * impl_->transform;
}
void GameObject::move_local_forward(float distance) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    Matrix moveMat = MatrixTranslate(0.0f, 0.0f, -distance);
    impl_->transform = moveMat * impl_->transform;
}

//================================================================================== 
// Rotation
void GameObject::rotate_pitch(float angle_rad) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    impl_->transform = MatrixRotateX(angle_rad) * impl_->transform;
}
void GameObject::rotate_yaw(float angle_rad) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    impl_->transform = MatrixRotateY(angle_rad) * impl_->transform;
}
void GameObject::rotate_roll(float angle_rad) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    impl_->transform = MatrixRotateZ(angle_rad) * impl_->transform;
}

void GameObject::rotate(float delta_pitch, float delta_yaw, float delta_roll) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    Matrix p_mat = MatrixRotateX(delta_pitch);
    Matrix y_mat = MatrixRotateY(delta_yaw);
    Matrix r_mat = MatrixRotateZ(delta_roll);

    Matrix frame_rotation = p_mat * y_mat * r_mat;

    impl_->transform = frame_rotation * impl_->transform;
}

//================================================================================== 
// Frame by frame behavior
void GameObject::on_update(Engine& engine){
    if (!impl_) impl_ = std::make_unique<Impl>();
    update(engine);
    
    float alpha = engine.get_interpolation_alpha();
    
    // Decompose transforms for proper interpolation
    Vector3 old_pos = { impl_->previous_transform.m12, impl_->previous_transform.m13, impl_->previous_transform.m14 };
    Vector3 curr_pos = { impl_->transform.m12, impl_->transform.m13, impl_->transform.m14 };
    Vector3 interp_pos = Vector3Lerp(old_pos, curr_pos, alpha);

    Quaternion old_rot = QuaternionFromMatrix(impl_->previous_transform);
    Quaternion curr_rot = QuaternionFromMatrix(impl_->transform);
    Quaternion interp_rot = QuaternionSlerp(old_rot, curr_rot, alpha);

    // TODO: Handle scale if needed. For now assuming uniform scale 1.0
    impl_->visual_transform = QuaternionToMatrix(interp_rot);
    impl_->visual_transform.m12 = interp_pos.x;
    impl_->visual_transform.m13 = interp_pos.y;
    impl_->visual_transform.m14 = interp_pos.z;

    if (impl_->component) {
        impl_->component->update();
    }
}

void GameObject::on_before_update(Engine& engine){
    before_update(engine);
}

void GameObject::on_after_update(Engine& engine){
    after_update(engine);
}

void GameObject::on_fixed_update(Engine& engine) {
    if (!impl_) impl_ = std::make_unique<Impl>();
    impl_->previous_transform = impl_->transform;
    fixed_update(engine);
}

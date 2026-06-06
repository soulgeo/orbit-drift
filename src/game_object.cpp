#include "game_object.hpp"
#include "raymath.h"

//================================================================================== 
// Global Movement

void GameObject::set_position_x(float x) {
    this->transform.m12 = x; 
}
void GameObject::set_position_y(float y) {
    this->transform.m13 = y; 
}
void GameObject::set_position_z(float z) {
    this->transform.m14 = z; 
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
void GameObject::move_global_x(float delta_x) { this->transform.m12 += delta_x; }
void GameObject::move_global_y(float delta_y) { this->transform.m13 += delta_y; }
void GameObject::move_global_z(float delta_z) { this->transform.m14 += delta_z; }

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
    return (Vector3){ transform.m12, transform.m13, transform.m14 }; 
}

Vector3 GameObject::get_right() const { 
    return (Vector3){ transform.m0,  transform.m1,  transform.m2 }; 
}
Vector3 GameObject::get_up() const { 
    return (Vector3){ transform.m4,  transform.m5,  transform.m6 }; 
}
Vector3 GameObject::get_forward() const { 
    return (Vector3){ -transform.m8, -transform.m9, -transform.m10 }; 
}

void GameObject::move_local_right(float distance) {
    Matrix moveMat = MatrixTranslate(distance, 0.0f, 0.0f);
    this->transform = moveMat * this->transform;
}
void GameObject::move_local_up(float distance) {
    Matrix moveMat = MatrixTranslate(0.0f, distance, 0.0f);
    this->transform = moveMat * this->transform;
}
void GameObject::move_local_forward(float distance) {
    Matrix moveMat = MatrixTranslate(0.0f, 0.0f, -distance);
    this->transform = moveMat * this->transform;
}

//================================================================================== 
// Rotation
void GameObject::rotate_pitch(float angle_rad) {
    this->transform = MatrixRotateX(angle_rad) * this->transform;
}
void GameObject::rotate_yaw(float angle_rad) {
    this->transform = MatrixRotateY(angle_rad) * this->transform;
}
void GameObject::rotate_roll(float angle_rad) {
    this->transform = MatrixRotateZ(angle_rad) * this->transform;
}

void GameObject::rotate(float delta_pitch, float delta_yaw, float delta_roll) {
    Matrix p_mat = MatrixRotateX(delta_pitch);
    Matrix y_mat = MatrixRotateY(delta_yaw);
    Matrix r_mat = MatrixRotateZ(delta_roll);

    Matrix frame_rotation = p_mat * y_mat * r_mat;

    this->transform = frame_rotation * this->transform;
}

//================================================================================== 
// Frame by frame behavior
void GameObject::on_update(Scene& scene){
    update(scene);
}

void GameObject::on_before_update(Scene& scene){
    before_update(scene);
}

void GameObject::on_after_update(Scene& scene){
    after_update(scene);
}

#include "physics_component.hpp"
#include "raylib.h"
#include "renderer.hpp"
#include "transform_component.hpp"
#include "debug_component.hpp"
#include "raymath.h"
#include <iostream>
#include <stdexcept>

PhysicsComponent::PhysicsComponent(
    GameObject* owner, Vector3 init_v, Vector3 init_a) : Component(owner) 
{
    v_ = init_v;
    a_ = init_a;
    mass_ = 0.0f;
    drag_ = 0.0f;
    is_kinematic_ = true;
}

PhysicsComponent::PhysicsComponent(
    GameObject* owner, Vector3 init_v, Vector3 init_a, float mass) : Component(owner) 
{
    v_ = init_v;
    a_ = init_a;
    mass_ = mass;
    drag_ = 0.0f;
    is_kinematic_ = false;
}

PhysicsComponent::~PhysicsComponent() = default;

Vector3 PhysicsComponent::velocity() {
    return v_;
}

Vector3 PhysicsComponent::acceleration() {
    return a_;
}

float PhysicsComponent::mass() {
    return mass_;
}

void PhysicsComponent::set_velocity(Vector3 v) {
    v_ = v;
}

void PhysicsComponent::set_acceleration(Vector3 a) {
    a_ = a;
}
DebugComponent* debug_;

void PhysicsComponent::set_mass(float mass) {
    mass_ = mass;
    is_kinematic_ = false;
}

void PhysicsComponent::set_drag(float drag) {
    if (is_kinematic_) {
        throw std::logic_error("Cannot set drag for kinematic component.");
    }
    drag_ = drag;
}

void PhysicsComponent::apply_force(Vector3 force) {
    forces_.push_back(force);
}

void PhysicsComponent::apply_force_local(Vector3 force) {
    Quaternion rot = QuaternionFromMatrix(transform_->get_transform());
    Vector3 world_force = Vector3RotateByQuaternion(force, rot);
    forces_.push_back(world_force);
}

void PhysicsComponent::start() {
    transform_ = owner_->get_component<TransformComponent>();
    debug_ = owner_->get_component<DebugComponent>();
}

void PhysicsComponent::fixed_update() {
    float f_dt = owner_->get_engine()->get_fixed_dt();

    if (!is_kinematic_) {
        apply_force(-drag_ * v_);
    }
    
    for (auto i = forces_.begin(); i != forces_.end(); i++) {
        a_ += (*i)/mass_;
    }

    v_ += a_ * f_dt;
    std::cout << "Acceleration x: " << a_.x << std::endl;
    std::cout << "Fixed dt: " << f_dt << std::endl;
    std::cout << "Velocity x: " << v_.x << std::endl;

    Vector3 current_pos = transform_->get_position();
    transform_->set_position(current_pos + (v_ * f_dt));
}

void PhysicsComponent::late_update() {
    if (debug_) {
        debug_->writeln(TextFormat("--- PHYSICS ---"));
        debug_->writeln(TextFormat("Velocity: %.2f, %.2f, %.2f",
                                   v_.x, v_.y, v_.z));
        debug_->writeln(TextFormat("Acceleration: %.2f, %.2f, %.2f",
                                   a_.x, a_.y, a_.z));
    }

    forces_.clear();
    a_ = Vector3Zero();
}

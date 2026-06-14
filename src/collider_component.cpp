#include "collider_component.hpp"
#include "game_object.hpp"
#include "transform_component.hpp"
#include "raymath.h"
#include "physics.hpp"

ColliderComponent::ColliderComponent(
    GameObject* owner, Physics* physics, Vector3 offset, float radius) : 
    Component(owner), offset_(offset) 
{
    physics->register_collider(this);
    type_ = Type::SPHERE;
    sphere_ = {radius};
}

ColliderComponent::ColliderComponent(
    GameObject* owner, Physics* physics, Vector3 offset, float radius, float height) : 
    Component(owner), offset_(offset) 
{
    physics->register_collider(this);
    type_ = Type::CAPSULE;
    capsule_ = {radius, height};
}

ColliderComponent::ColliderComponent(
    GameObject* owner, Physics* physics, Vector3 offset, Vector3 half_extents) : 
    Component(owner), offset_(offset) 
{
    physics->register_collider(this);
    type_ = Type::BOX;
    box_ = {half_extents};
}

ColliderComponent::~ColliderComponent() = default;

void ColliderComponent::start() {
    transform_ = owner_->get_component<TransformComponent>();
}

void ColliderComponent::fixed_update() {
    center_ = transform_->get_position() + offset_;
}

#include <algorithm>
#include "sputnik/physics/collider_component.hpp"
#include "sputnik/ecs/game_object.hpp"
#include <raylib.h>
#include "sputnik/ecs/transform_component.hpp"
#include <raymath.h>
#include "sputnik/physics/physics.hpp"

namespace Sputnik {

    ColliderComponent::ColliderComponent(
        GameObject* owner, Physics* physics, 
        Vector3 offset, float radius, 
        bool is_trigger) : 
        Component(owner), offset_(offset), is_trigger_(is_trigger)
    {
        physics->register_collider(this);
        type_ = Type::SPHERE;
        shape_.sphere = {radius};
    }

    ColliderComponent::ColliderComponent(
        GameObject* owner, Physics* physics, 
        Vector3 offset, Vector3 half_extents,
        bool is_trigger) : 
        Component(owner), offset_(offset), is_trigger_(is_trigger)
    {
        physics->register_collider(this);
        type_ = Type::BOX;
        shape_.box = {half_extents};
    }

    ColliderComponent::~ColliderComponent() = default;

    bool ColliderComponent::is_trigger() const {
        return is_trigger_;
    }

    ColliderComponent::Type ColliderComponent::type() const {
        return type_;
    }

    Vector3 ColliderComponent::center() const {
        return center_;
    }

    ColliderComponent::Shape::Sphere ColliderComponent::sphere() const {
        return shape_.sphere;
    }

    ColliderComponent::Shape::Box ColliderComponent::box() const {
        return shape_.box;
    }

    GameObject* ColliderComponent::owner() {
        return owner_;
    }

    size_t ColliderComponent::tracked_collider_count() const {
        return tracked_collisions_.size();
    }

    ColliderComponent* ColliderComponent::tracked_collider_at(size_t index) const {
        return tracked_collisions_[index];
    }

    bool ColliderComponent::is_colliding_with(ColliderComponent* other) {
        bool colliding = false;

        if (type_ == Type::BOX) {
            BoundingBox this_box = {
                center_ - box().half_extents, 
                center_ + box().half_extents
            };
            if (other->type() == Type::BOX) {
                BoundingBox other_box = {
                    other->center() - other->box().half_extents, 
                    other->center() + other->box().half_extents
                };
                colliding = CheckCollisionBoxes(this_box, other_box);
            }
            else if (other->type() == Type::SPHERE) {
                colliding = CheckCollisionBoxSphere(
                    this_box, other->center(), other->sphere().radius
                );
            }
        }
        else if (type_ == Type::SPHERE) {
            if (other->type() == Type::BOX) {
                BoundingBox other_box = {
                    other->center() - other->box().half_extents, 
                    other->center() + other->box().half_extents
                };
                colliding = CheckCollisionBoxSphere(
                    other_box, center_, sphere().radius
                );
            }
            else if (other->type() == Type::SPHERE) {
                colliding = CheckCollisionSpheres(
                    center_, sphere().radius, other->center(), other->sphere().radius
                );
            }
        }
        return colliding;
    }

    bool ColliderComponent::is_tracking(ColliderComponent* other) const {
        for (auto tracked : tracked_collisions_) {
            if (tracked == other) return true;
        }
        return false;
    }

    void ColliderComponent::add_tracked_collider(ColliderComponent* collider) {
        tracked_collisions_.push_back(collider);
    }

    void ColliderComponent::remove_tracked_collider(ColliderComponent* collider) {
        tracked_collisions_.erase(
            std::remove(tracked_collisions_.begin(), tracked_collisions_.end(), collider), 
            tracked_collisions_.end()
        );
    }

    void ColliderComponent::start() {
        transform_ = owner_->component<TransformComponent>();
    }

    void ColliderComponent::fixed_update() {
        center_ = transform_->position() + offset_;
    }

}

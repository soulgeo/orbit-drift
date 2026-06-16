#include "sputnik/physics/physics.hpp"
#include "sputnik/physics/collider_component.hpp"
#include "sputnik/physics/physics_component.hpp"
#include <algorithm>
#include "sputnik/core/engine.hpp"

namespace Sputnik {

    Physics::Physics() {}
    Physics::~Physics() {}

    void Physics::register_body(PhysicsComponent* body) {
        bodies_.push_back(body);
    }

    void Physics::unregister_body(PhysicsComponent* body) {
        bodies_.erase(
            std::remove(bodies_.begin(), bodies_.end(), body), 
            bodies_.end()
        );
    }

    void Physics::register_collider(ColliderComponent* collider) {
        colliders_.push_back(collider);
    }

    void Physics::unregister_collider(ColliderComponent* collider) {
        colliders_.erase(
            std::remove(colliders_.begin(), colliders_.end(), collider), 
            colliders_.end()
        );
    }

    void Physics::update(Engine* engine) {
        float f_dt = engine->fixed_dt();

        for (auto i = bodies_.begin(); i != bodies_.end(); i++) {
            (*i)->calculate_physics(f_dt);
        }

        for (size_t i = 0; i < colliders_.size(); ++i) {
            for (size_t j = i + 1; j < colliders_.size(); ++j) {
                auto a = colliders_[i];
                auto b = colliders_[j];
                if (a->is_colliding_with(b)){
                    if (!a->is_tracking(b)){ handle_collision_enter(a, b); }
                    handle_collision_stay(a, b);
                } else if (a->is_tracking(b)){
                    handle_collision_exit(a, b);
                }
            }
        }
    }

    void Physics::handle_collision_enter(
        ColliderComponent* collider_a, ColliderComponent* collider_b)
    {
        collider_a->add_tracked_collider(collider_b);
        collider_b->add_tracked_collider(collider_a);

        GameObject* obj_a = collider_a->owner();
        GameObject* obj_b = collider_b->owner();

        if (!collider_a->is_trigger() && !collider_b->is_trigger()){
            obj_a->on_collision_enter(obj_b);
            obj_b->on_collision_enter(obj_a);
        }
        else {
            if (collider_a->is_trigger()){
                obj_a->on_trigger_enter(obj_b);
            }
            if (collider_b->is_trigger()){
                obj_b->on_trigger_enter(obj_a);
            }
        }            
    }

    void Physics::handle_collision_stay(
        ColliderComponent* collider_a, ColliderComponent* collider_b)
    {
        GameObject* obj_a = collider_a->owner();
        GameObject* obj_b = collider_b->owner();
        if (collider_a->is_trigger()){
            obj_a->on_trigger_stay(obj_b);
        }
        if (collider_b->is_trigger()){
            obj_b->on_trigger_stay(obj_a);
        }
    }

    void Physics::handle_collision_exit(
        ColliderComponent* collider_a, ColliderComponent* collider_b)
    {
        GameObject* obj_a = collider_a->owner();
        GameObject* obj_b = collider_b->owner();
        if (!collider_a->is_trigger() && !collider_b->is_trigger()){
            obj_a->on_collision_exit(obj_b);
            obj_b->on_collision_exit(obj_a);
        }
        else {
            if (collider_a->is_trigger()){
                obj_a->on_trigger_exit(obj_b);
            }
            if (collider_b->is_trigger()){
                obj_b->on_trigger_exit(obj_a);
            }
        }            
        collider_a->remove_tracked_collider(collider_b);
        collider_b->remove_tracked_collider(collider_a);
    }

}

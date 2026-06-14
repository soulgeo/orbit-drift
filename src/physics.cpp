#include "physics.hpp"
#include "collider_component.hpp"
#include "physics_component.hpp"
#include <algorithm>

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
    float f_dt = engine->get_fixed_dt();

    for (auto i = bodies_.begin(); i != bodies_.end(); i++) {
        (*i)->calculate_physics(f_dt);
    }
}

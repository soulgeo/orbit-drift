#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <vector>
#include "sputnik/physics/physics_component.hpp"
#include "sputnik/physics/collider_component.hpp"

namespace Sputnik {

    class Physics {
        std::vector<PhysicsComponent*> bodies_;
        std::vector<ColliderComponent*> colliders_;
        
        void handle_collision_enter(
            ColliderComponent* collider_a, ColliderComponent* collider_b);
        void handle_collision_stay(
            ColliderComponent* collider_a, ColliderComponent* collider_b);
        void handle_collision_exit(
            ColliderComponent* collider_a, ColliderComponent* collider_b);

    public:
        Physics();
        ~Physics();

        void register_body(PhysicsComponent* body);
        void unregister_body(PhysicsComponent* body);

        void register_collider(ColliderComponent* collider);
        void unregister_collider(ColliderComponent* collider);

        void update(Engine* engine);
    };

}

#endif // !PHYSICS_HPP

#pragma once

#include "sputnik/ecs/component.hpp"
#include "sputnik/ecs/game_object.hpp"
#include <raylib.h>
#include <vector>

namespace Sputnik {

    class Physics;

    class ColliderComponent : public Component {
        bool is_trigger_;

        TransformComponent* transform_;
        Vector3 offset_;

        Vector3 center_;
        enum class Type { SPHERE, BOX };
        Type type_;
        union Shape {
            struct Sphere { float radius; } sphere;
            struct Box { Vector3 half_extents; } box;
        } shape_;

        // TODO: Handle potential pointer-related issues
        std::vector<ColliderComponent*> tracked_collisions_; 

    public:
        // SPHERE
        ColliderComponent(
            GameObject* owner, Physics* physics, 
            Vector3 offset, float radius, 
            bool is_trigger
        );
        // BOX
        ColliderComponent(
            GameObject* owner, Physics* physics, 
            Vector3 offset, Vector3 half_extents, 
            bool is_trigger
        );

        ~ColliderComponent();

        bool is_trigger() const;
        Type type() const;
        Vector3 center() const;

        Shape::Sphere sphere() const;
        Shape::Box box() const;

        GameObject* owner();

        bool is_colliding_with(ColliderComponent* other);
        bool is_tracking(ColliderComponent* other) const;

        void add_tracked_collider(ColliderComponent* collider);
        void remove_tracked_collider(ColliderComponent* collider);

        size_t tracked_collider_count() const;
        ColliderComponent* tracked_collider_at(size_t index) const;

        void init() override;
        void fixed_update() override;
    };

}

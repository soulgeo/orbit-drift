#ifndef COLLIDER_COMPONENT_HPP
#define COLLIDER_COMPONENT_HPP

#include "component.hpp"
#include "game_object.hpp"
#include "raylib.h"

class Physics;

class ColliderComponent : public Component {
    TransformComponent* transform_;
    Vector3 offset_;

    Vector3 center_;
    enum class Type { SPHERE, BOX, CAPSULE };
    Type type_;
    union {
        struct { float radius; } sphere_;
        struct { Vector3 half_extents; } box_;
        struct { float radius; float height; } capsule_;
    };

public:
    ColliderComponent(
        GameObject* owner, Physics* physics, Vector3 offset, float radius);
    ColliderComponent(
        GameObject* owner, Physics* physics, Vector3 offset, float radius, float height);
    ColliderComponent(
        GameObject* owner, Physics* physics, Vector3 offset, Vector3 half_extents);
    ~ColliderComponent();

    void start() override;
    void fixed_update() override;
};

#endif // !COLLIDER_COMPONENT_HPP

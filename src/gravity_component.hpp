#ifndef GRAVITY_COMPONENT_HPP
#define GRAVITY_COMPONENT_HPP

#include "component.hpp"
#include "debug_component.hpp"
#include "game_object.hpp"
#include "physics_component.hpp"
#include "raylib.h"

class GravityComponent : public Component {
    Vector3 center_;
    float radius_;
    float force_amp_;

    bool detected_player_;

    TransformComponent* transform_;
    DebugComponent* debug_;

    TransformComponent* player_transform_;
    PhysicsComponent* player_physics_;

public:
    GravityComponent(GameObject* owner, float radius, float force_amp);
    ~GravityComponent();

    void start() override;
    void fixed_update() override;
    void late_update() override;
};

#endif // !GRAVITY_COMPONENT_HPP

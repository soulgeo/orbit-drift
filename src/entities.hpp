#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include "component.hpp"
#include "debug_component.hpp"
#include "raylib.h"
#include "transform_component.hpp"

class PlayerShipComponent : public Component {
    float curr_roll_speed_ = 0.0f;
    Vector3 curr_velocity_;
    Vector3 ext_gravity_velocity_;
    BoundingBox hitbox_;
    TransformComponent* transform_;
    DebugComponent* debug_;

public:
    PlayerShipComponent(GameObject* owner);
    ~PlayerShipComponent();

    float forward_speed = 0.3f;
    float forward_accel = 1.5f;

    float right_speed = 0.15f;
    float right_accel = 3.0f;

    float up_speed = 0.15f;
    float up_accel = 3.0f;

    float pan_speed = 0.004f;

    float roll_speed = 1.1f;
    float roll_accel = 4.0f;

    bool g_flag = false;
    bool in_gravity = false;
    bool entered_gravity = false;
    bool exited_gravity = false;

    BoundingBox get_hitbox();

    void add_gravity(Vector3 gravityAccel);

    void start() override;

    void early_update() override;
    void fixed_update() override;
    void update() override;
    void late_update() override;
};

class PlanetComponent : public Component {
    float radius_;
    float gravity_radius_;
    float gravity_force_;
    float rotation_speed_;
    Vector3 init_position_;

    TransformComponent* transform_;

    PlayerShipComponent* player_ship_;
    TransformComponent* player_transform_;

public:
    PlanetComponent(GameObject* owner, Vector3 position, float radius, float gravity_radius, float gravity_force);
    ~PlanetComponent();

    void start() override;

    void fixed_update() override;
    void update() override;
};

#endif // !ENTITIES_HPP

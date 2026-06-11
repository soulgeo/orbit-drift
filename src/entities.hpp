#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include "game_object.hpp"
#include "raylib.h"

class PlayerShip : public GameObject {
public:
    PlayerShip();
    ~PlayerShip();

    float forward_speed = 0.8f;
    float forward_accel = 1.5f;

    float right_speed = 0.4f;
    float right_accel = 3.0f;

    float up_speed = 0.4f;
    float up_accel = 3.0f;

    float pan_speed = 0.004f;

    float roll_speed = 3.0f;
    float roll_accel = 4.0f;

    bool g_flag = false;
    bool in_gravity = false;
    bool entered_gravity = false;
    bool exited_gravity = false;

    BoundingBox get_hitbox();

    void add_gravity(Vector3 gravityAccel);

    void update(Engine& engine) override;
    void before_update(Engine& engine) override;
    void after_update(Engine& engine) override;
    void fixed_update(Engine& engine) override;

private:
    float curr_roll_speed_ = 0.0f;
    Vector3 curr_velocity_;
    Vector3 ext_gravity_velocity_;
    BoundingBox hitbox_;
};

class Planet : public GameObject {
public:
    Planet(Vector3 position, float radius, float gravity_radius, float gravity_force);
    ~Planet();

    float radius;
    float gravity_radius;
    float gravity_force;
    float rotation_speed;

    void update(Engine& engine) override;
    void fixed_update(Engine& engine) override;
};

#endif // !ENTITIES_HPP

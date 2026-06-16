#ifndef PHYSICS_COMPONENT_HPP
#define PHYSICS_COMPONENT_HPP

#include "component.hpp"
#include "raylib.h"
#include "renderer.hpp"
#include "transform_component.hpp"
#include <vector>

class Physics;

class PhysicsComponent : public Component {
    Vector3 v_;
    Vector3 a_;
    float mass_;
    float drag_;
    bool is_kinematic_;

    std::vector<Vector3> forces_;

    TransformComponent* transform_;
    DebugComponent* debug_;

public:
    PhysicsComponent(
        GameObject* owner, Physics* physics, Vector3 init_v, Vector3 init_a);
    PhysicsComponent(
        GameObject* owner, Physics* physics, Vector3 init_v, Vector3 init_a, float mass);
    ~PhysicsComponent();

    Vector3 velocity();
    Vector3 acceleration();
    float mass();
    float drag();
    bool is_kinematic();

    TransformComponent* transform();

    size_t get_force_count() const;
    Vector3 get_force_at(size_t index) const;

    // void set_velocity(Vector3 v);
    // void set_acceleration(Vector3 a);
    // void set_mass(float mass);
    void set_drag(float drag);
    void calculate_physics(float dt);

    void apply_force(Vector3 force);
    void apply_force_local(Vector3 force);
    void clear_forces();

    void start() override;
    // void fixed_update() override;
    void late_update() override;
};

#endif // !PHYSICS_COMPONENT_HPP


#ifndef CONTROL_COMPONENT_HPP
#define CONTROL_COMPONENT_HPP

#include "sputnik/ecs/component.hpp"
#include "sputnik/physics/physics_component.hpp"
#include "sputnik/ecs/transform_component.hpp"

class ControlComponent : public Sputnik::Component {
    int x_axis_;
    int y_axis_;
    int z_axis_;
    int roll_axis_;
    Vector2 pan_;

    float x_amp_;
    float y_amp_;
    float z_amp_;
    float roll_amp_;
    float pan_amp_;

    Sputnik::PhysicsComponent* physics_;
    Sputnik::TransformComponent* transform_;

public:
    ControlComponent(Sputnik::GameObject* owner);
    ~ControlComponent();

    void start() override;
    void early_update() override;
    void fixed_update() override;
};

#endif // !CONTROL_COMPONENT_HPP

#ifndef CONTROL_COMPONENT_HPP
#define CONTROL_COMPONENT_HPP

#include "component.hpp"
#include "physics_component.hpp"
#include "transform_component.hpp"

class ControlComponent : public Component {
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

    PhysicsComponent* physics_;
    TransformComponent* transform_;

public:
    ControlComponent(GameObject* owner);
    ~ControlComponent();

    void start() override;
    void early_update() override;
    void fixed_update() override;
};

#endif // !CONTROL_COMPONENT_HPP

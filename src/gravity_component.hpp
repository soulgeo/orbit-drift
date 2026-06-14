#ifndef GRAVITY_COMPONENT_HPP
#define GRAVITY_COMPONENT_HPP

#include "component.hpp"
#include "debug_component.hpp"
#include "game_object.hpp"
#include "raylib.h"
#include "renderer.hpp"

class GravityComponent : public Component {
    Vector3 center_;
    float radius_;
    float force_amp_;

    TransformComponent* transform_;
    DebugComponent* debug_;

    CameraComponent* camera_;

public:
    GravityComponent(GameObject* owner, float radius, float force_amp);
    ~GravityComponent();

    void start() override;
    void late_update() override;

    void on_trigger_enter(GameObject* other) override;
    void on_trigger_stay(GameObject* other) override;
    void on_trigger_exit(GameObject* other) override;
};

#endif // !GRAVITY_COMPONENT_HPP

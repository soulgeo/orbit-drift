#ifndef GRAVITY_COMPONENT_HPP
#define GRAVITY_COMPONENT_HPP

#include "sputnik/ecs/component.hpp"
#include "sputnik/rendering/debug_component.hpp"
#include "sputnik/ecs/game_object.hpp"
#include <raylib.h>
#include "sputnik/rendering/renderer.hpp"

class GravityComponent : public Sputnik::Component {
    Vector3 center_;
    float radius_;
    float force_amp_;

    Sputnik::TransformComponent* transform_;
    Sputnik::DebugComponent* debug_;

    Sputnik::CameraComponent* camera_;

public:
    GravityComponent(Sputnik::GameObject* owner, float radius, float force_amp);
    ~GravityComponent();

    void start() override;
    void late_update() override;

    void on_trigger_enter(Sputnik::GameObject* other) override;
    void on_trigger_stay(Sputnik::GameObject* other) override;
    void on_trigger_exit(Sputnik::GameObject* other) override;

    float force_amp() const;
};

#endif // !GRAVITY_COMPONENT_HPP

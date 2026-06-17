#ifndef GRAVITY_COMPONENT_HPP
#define GRAVITY_COMPONENT_HPP

#include "events/event_component.hpp"
#include <raylib.h>
#include <sputnik.hpp>

class GravityComponent : public Sputnik::Component {
    Vector3 center_;
    float radius_;
    float force_amp_;

    Sputnik::TransformComponent* transform_;
    Sputnik::EventComponent* event_;
    Sputnik::DebugComponent* debug_;

    Sputnik::CameraComponent* camera_;

public:
    GravityComponent(Sputnik::GameObject* owner, float radius, float force_amp);
    ~GravityComponent();

    void init() override;
    void late_update() override;

    void on_trigger_enter(Sputnik::GameObject* other) override;
    void on_trigger_stay(Sputnik::GameObject* other) override;
    void on_trigger_exit(Sputnik::GameObject* other) override;

    float force_amp() const;
};

#endif // !GRAVITY_COMPONENT_HPP

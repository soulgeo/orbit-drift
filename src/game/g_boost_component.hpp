#ifndef GBOOST_COMPONENT_HPP
#define GBOOST_COMPONENT_HPP

#include "sputnik/ecs/component.hpp"
#include "sputnik/ecs/game_object.hpp"
#include "sputnik/physics/physics_component.hpp"

class GBoostComponent : public Sputnik::Component {
    Sputnik::PhysicsComponent* physics_;
    float boost_amp_;

public:
    GBoostComponent(Sputnik::GameObject* owner);
    ~GBoostComponent();

    void start() override;
    void on_trigger_stay(Sputnik::GameObject* other) override;
    void on_trigger_exit(Sputnik::GameObject* other) override;

    float boost_amp() const;

    void set_boost_amp(float boost_amp);
};

#endif // !GBOOST_COMPONENT_HPP

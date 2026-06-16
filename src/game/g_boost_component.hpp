#ifndef GBOOST_COMPONENT_HPP
#define GBOOST_COMPONENT_HPP

#include "component.hpp"
#include "game_object.hpp"
#include "physics_component.hpp"

class GBoostComponent : public Component {
    PhysicsComponent* physics_;
    float boost_amp_;

public:
    GBoostComponent(GameObject* owner);
    ~GBoostComponent();

    void start() override;
    void on_trigger_stay(GameObject* other) override;
    void on_trigger_exit(GameObject* other) override;

    float boost_amp() const;

    void set_boost_amp(float boost_amp);
};

#endif // !GBOOST_COMPONENT_HPP

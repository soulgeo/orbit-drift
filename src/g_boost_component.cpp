#include "g_boost_component.hpp"
#include "gravity_component.hpp"

GBoostComponent::GBoostComponent(GameObject* owner) : Component(owner) {
    boost_amp_ = 500.0f;
}

GBoostComponent::~GBoostComponent() = default;

void GBoostComponent::start() {
    physics_ = owner_->get_component<PhysicsComponent>();
}

void GBoostComponent::on_trigger_stay(GameObject* other) {
    auto gravity = other->get_component<GravityComponent>();
    if (!gravity || !physics_) return;

    physics_->apply_force_local({0.0f, 0.0f, -boost_amp_});
}

void GBoostComponent::on_trigger_exit(GameObject* other) {
    
}

float GBoostComponent::boost_amp() const {
    return boost_amp_;
}

void GBoostComponent::set_boost_amp(float boost_amp) {
    boost_amp_ = boost_amp;
}

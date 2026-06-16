#include "g_boost_component.hpp"
#include "gravity_component.hpp"

using namespace Sputnik;

GBoostComponent::GBoostComponent(GameObject* owner) : Component(owner) {
    boost_amp_ = 1000.0f;
}

GBoostComponent::~GBoostComponent() = default;

void GBoostComponent::start() {
    physics_ = owner_->component<PhysicsComponent>();
}

void GBoostComponent::on_trigger_stay(GameObject* other) {
    auto gravity = other->component<GravityComponent>();
    if (!gravity || !physics_) return;

    float gravity_amp = gravity->force_amp();
    float final_boost_amp = boost_amp_ + gravity_amp * 1/40;

    physics_->apply_force_local({0.0f, 0.0f, -final_boost_amp});
}

void GBoostComponent::on_trigger_exit(GameObject* other) {
    
}

float GBoostComponent::boost_amp() const {
    return boost_amp_;
}

void GBoostComponent::set_boost_amp(float boost_amp) {
    boost_amp_ = boost_amp;
}

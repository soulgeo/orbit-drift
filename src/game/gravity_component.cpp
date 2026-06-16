#include "gravity_component.hpp"
#include "sputnik/rendering/camera_component.hpp"
#include "sputnik/control_component.hpp"
#include "sputnik/physics/physics_component.hpp"
#include <raylib.h>
#include <raymath.h>
#include "sputnik/ecs/transform_component.hpp"
#include "sputnik/core/engine.hpp"

using namespace Sputnik;

GravityComponent::GravityComponent(GameObject* owner, float radius, float force_amp) :
    Component(owner),
    radius_(radius),
    force_amp_(force_amp)
{}

GravityComponent::~GravityComponent() {}

void GravityComponent::start() {
    transform_ = owner_->get_component<TransformComponent>();
    debug_ = owner_->get_component<DebugComponent>();

    Scene& scene = owner_->get_engine()->get_scene();
    GameObject* camera_body = scene.get_game_object("camera_body");
    camera_ = camera_body->get_component<CameraComponent>();
}

void GravityComponent::late_update() {
    // if (!debug_) return;
    //
    // debug_->writeln(TextFormat("--- GRAVITY ---"));
    // debug_->writeln(TextFormat("Player detected: %s", detected_player_ ? "TRUE" : "FALSE"));
}

void GravityComponent::on_trigger_enter(GameObject* other) {
    auto control = other->get_component<ControlComponent>();
    if (!control) return;
    auto camera_profile = camera_->get_profile_id();
    if (camera_profile == CameraComponent::CP_DEFAULT) {
        camera_->switch_profile(CameraComponent::CP_IN_GRAVITY);
    }
}

void GravityComponent::on_trigger_stay(GameObject* other) {
    auto other_physics = other->get_component<PhysicsComponent>();
    if (!other_physics) return;

    auto other_transform = other->get_component<TransformComponent>();
    auto other_position = other_transform->get_position();

    center_ = transform_->get_position();

    float distance = Vector3Distance(center_, other_position);
    float distanceFactor = 1.0f - Clamp(distance / radius_, 0.8f, 1.0f);
    Vector3 direction = Vector3Normalize(center_ - other_position);
    Vector3 force = direction * (force_amp_ * distanceFactor);
    other_physics->apply_force(force);
}

void GravityComponent::on_trigger_exit(GameObject* other) {
    auto control = other->get_component<ControlComponent>();
    if (!control) return;
    auto camera_profile = camera_->get_profile_id();
    if (camera_profile == CameraComponent::CP_IN_GRAVITY) {
        camera_->switch_profile(CameraComponent::CP_DEFAULT);
    }
}

float GravityComponent::force_amp() const {
    return force_amp_;
}

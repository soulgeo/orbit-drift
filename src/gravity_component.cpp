#include "gravity_component.hpp"
#include "raylib.h"
#include "raymath.h"

GravityComponent::GravityComponent(GameObject* owner, float radius, float force_amp) :
    Component(owner),
    radius_(radius),
    force_amp_(force_amp),
    detected_player_(false)
{}

GravityComponent::~GravityComponent() {}

void GravityComponent::start() {
    transform_ = owner_->get_component<TransformComponent>();
    debug_ = owner_->get_component<DebugComponent>();

    Scene& scene = owner_->get_engine()->get_scene();
    GameObject* player = scene.get_game_object("player");
    player_physics_ = player->get_component<PhysicsComponent>();
    player_transform_ = player->get_component<TransformComponent>();
}

void GravityComponent::fixed_update() {
    if (!player_physics_ || !player_transform_ || !transform_) { return; }

    center_ = transform_->get_position();
    Vector3 player_position = player_transform_->get_position();
    
    detected_player_ = CheckCollisionSpheres(
        player_position, 0.2f, center_, radius_);
        
    if (detected_player_) {
        float distance = Vector3Distance(center_, player_position);
        float distanceFactor = 1.0f - Clamp(distance / radius_, 0.1f, 1.0f);
        Vector3 direction = Vector3Normalize(center_ - player_position);
        Vector3 force = force_amp_ * distanceFactor * direction;
        player_physics_->apply_force(force);
    }
}

void GravityComponent::late_update() {
    if (!debug_) return;

    debug_->writeln(TextFormat("--- GRAVITY ---"));
    debug_->writeln(TextFormat("Player detected: %s", detected_player_ ? "TRUE" : "FALSE"));
}

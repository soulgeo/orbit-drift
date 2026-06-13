#include "entities.hpp"

#include "debug_component.hpp"
#include "engine.hpp"
#include "raylib.h"
#include "raymath.h"
#include "transform_component.hpp"

float _smooth_towards(
    float start, float target, float speed, float dt, float threshold = 0.001f
) {
    float value = Lerp(start, target, speed*dt);
    if (std::abs(value - target) < threshold) return target;
    return value;
}

Vector3 _vector_3_smooth_towards(
    Vector3 start, Vector3 target, float speed, float dt, float threshold = 0.001f
) {
    Vector3 value = Vector3Lerp(start, target, speed*dt);
    if (Vector3Distance(value, target) < threshold) return target;
    return value;
}

PlayerShipComponent::PlayerShipComponent(GameObject* owner) : 
    Component(owner), 
    curr_velocity_(Vector3Zero()), 
    ext_gravity_velocity_(Vector3Zero()),
    hitbox_((BoundingBox){
        (Vector3){-0.2f, -0.2f, -0.2f}, (Vector3){0.2f, 0.2f, 0.2f}
    }) 
{}

PlayerShipComponent::~PlayerShipComponent() {}

void PlayerShipComponent::start() {
    transform_ = &owner_->transform();
    debug_ = owner_->get_component<DebugComponent>();
}

void PlayerShipComponent::early_update() {
    g_flag = false;
    entered_gravity = false;
}

void PlayerShipComponent::fixed_update() {
    Engine* engine = owner_->get_engine();

    float fixed_dt = engine->get_fixed_dt();
    Vector2 mouse_position = GetMousePosition();
    Vector2 mouse_distance = {mouse_position.x - 960, mouse_position.y - 540};
    float local_yaw = mouse_distance.x * -pan_speed * fixed_dt;
    float local_pitch = mouse_distance.y * -pan_speed * fixed_dt;

    transform_->rotate_pitch(local_pitch);
    transform_->rotate_yaw(local_yaw);

    int moveX =
        engine->is_active_input(INPUT_MOVE_RIGHT) - engine->is_active_input(INPUT_MOVE_LEFT);
    int moveY = engine->is_active_input(INPUT_MOVE_UP) - engine->is_active_input(INPUT_MOVE_DOWN);
    int moveZ =
        engine->is_active_input(INPUT_MOVE_FORWARD) - engine->is_active_input(INPUT_MOVE_BACK);
    int roll = 
        engine->is_active_input(INPUT_ROLL_CCW) - engine->is_active_input(INPUT_ROLL_CW);

    // Position Movement
    Vector3 localTargetVelocity = 
        {moveX * right_speed, moveY * up_speed, -moveZ * forward_speed};
    curr_velocity_ = _vector_3_smooth_towards(
        curr_velocity_, localTargetVelocity, fixed_dt, forward_accel
    );
    Quaternion rot = QuaternionFromMatrix(transform_->get_transform());

    Vector3 worldMovement = Vector3RotateByQuaternion(curr_velocity_, rot);
    worldMovement = worldMovement + ext_gravity_velocity_*fixed_dt;

    transform_->move_global(worldMovement);

    // Rotation
    curr_roll_speed_ = 
        _smooth_towards(curr_roll_speed_, roll*roll_speed, fixed_dt, roll_accel);
    transform_->rotate_roll(curr_roll_speed_*fixed_dt);

    ext_gravity_velocity_ = Vector3Zero();

    // Update hitbox position
    Vector3 pos = transform_->get_position();
    hitbox_.min = (Vector3){pos.x - 0.2f, pos.y - 0.2f, pos.z - 0.2f};
    hitbox_.max = (Vector3){pos.x + 0.2f, pos.y + 0.2f, pos.z + 0.2f};
}

void PlayerShipComponent::update() {}


void PlayerShipComponent::late_update(){
    exited_gravity = false;
    if (in_gravity && !g_flag) {
        in_gravity = false;
        exited_gravity = true;
    }

    Vector3 position = transform_->get_position();
    Vector3 up = transform_->get_up();
    if (debug_){
        debug_->writeln(TextFormat("--- SHIP ---"));
        debug_->writeln(TextFormat("Position: %.2f, %.2f, %.2f", 
                                   position.x, position.y, position.z));
        debug_->writeln(TextFormat("Up Vector: %.2f, %.2f, %.2f", 
                                   up.x, up.y, up.z));
        debug_->writeln(TextFormat("Velocity: %.2f, %.2f, %.2f", 
                                   curr_velocity_.x, curr_velocity_.y, curr_velocity_.z));
        debug_->writeln(TextFormat("Roll Speed: %.2f", curr_roll_speed_));
        debug_->writeln(TextFormat("Gravity: %s", in_gravity ? "YES" : "NO"));
    }
}

BoundingBox PlayerShipComponent::get_hitbox() {
    return hitbox_;
}

void PlayerShipComponent::add_gravity(Vector3 gravityAccel) {
    ext_gravity_velocity_ += gravityAccel;
}

PlanetComponent::PlanetComponent(GameObject* owner, Vector3 p_position, float p_radius, float p_gravityRadius, float p_gravityForce) :
    Component(owner),
    radius_(p_radius),
    gravity_radius_(p_gravityRadius),
    gravity_force_(p_gravityForce),
    init_position_(p_position)
{}

PlanetComponent::~PlanetComponent() {}

void PlanetComponent::start() {
    transform_ = owner_->get_component<TransformComponent>();
    transform_->set_position(init_position_);

    Scene& scene = owner_->get_engine()->get_scene();
    GameObject* player = scene.get_game_object("player");
    player_ship_ = player->get_component<PlayerShipComponent>();
    player_transform_ = player->get_component<TransformComponent>();
}

void PlanetComponent::fixed_update() {
    Engine* engine = owner_->get_engine();
    auto& scene = engine->get_scene();
    GameObject* player = scene.get_game_object("player");
    bool colliding = CheckCollisionBoxSphere(
        player_ship_->get_hitbox(), transform_->get_position(), gravity_radius_
    );
    if (colliding) {
        if (!player_ship_->in_gravity){
            player_ship_->entered_gravity = true;
        }
        player_ship_->g_flag = true;
        player_ship_->in_gravity = true;
        Vector3 direction = Vector3Normalize(
                transform_->get_position() - player_transform_->get_position()
            );
        float distance = Vector3Distance(
            player_transform_->get_position(), transform_->get_position()
        );
        float distanceFactor = 1.0f - Clamp(distance / gravity_radius_, 0, 1);
        Vector3 gravityAccel = gravity_force_ * distanceFactor * direction;

        player_ship_->add_gravity(gravityAccel);
    }
}

void PlanetComponent::update() {}

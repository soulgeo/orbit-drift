#include "entities.hpp"

#include "engine.hpp"
#include "raylib.h"
#include "raymath.h"

float _smooth_towards(float start, float target, float speed, float dt, float threshold = 0.001f) {
    float value = Lerp(start, target, speed*dt);
    if (std::abs(value - start) < threshold) return start;
    if (std::abs(value - target) < threshold) return target;
    return value;
}

Vector3 _vector_3_smooth_towards(Vector3 start, Vector3 target, float speed, float dt, float threshold = 0.001f) {
    Vector3 value = Vector3Lerp(start, target, speed*dt);
    if (Vector3Distance(value, start) < threshold) return start;
    if (Vector3Distance(value, target) < threshold) return target;
    return value;
}

PlayerShip::PlayerShip() {
    hitbox = (BoundingBox){(Vector3){-0.2f, -0.2f, -0.2f}, (Vector3){0.2f, 0.2f, 0.2f}};
}

PlayerShip::~PlayerShip() {}

void PlayerShip::before_update(Engine& engine) {
    g_flag = false;
    entered_gravity = false;
}

void PlayerShip::update(Engine& engine) {}

void PlayerShip::fixed_update(Engine& engine) {
    float fixed_dt = engine.get_fixed_dt();
    Vector2 mouse_position = GetMousePosition();
    Vector2 mouse_distance = {mouse_position.x - 960, mouse_position.y - 540};
    float local_yaw = mouse_distance.x * -pan_speed * fixed_dt;
    float local_pitch = mouse_distance.y * -pan_speed * fixed_dt;

    this->rotate_pitch(local_pitch);
    this->rotate_yaw(local_yaw);

    int moveX =
        engine.is_active_input(INPUT_MOVE_RIGHT) - engine.is_active_input(INPUT_MOVE_LEFT);
    int moveY = engine.is_active_input(INPUT_MOVE_UP) - engine.is_active_input(INPUT_MOVE_DOWN);
    int moveZ =
        engine.is_active_input(INPUT_MOVE_FORWARD) - engine.is_active_input(INPUT_MOVE_BACK);
    int roll = 
        engine.is_active_input(INPUT_ROLL_CCW) - engine.is_active_input(INPUT_ROLL_CW);

    // Position Movement
    Vector3 localTargetVelocity = {moveX * right_speed, moveY * up_speed, -moveZ * forward_speed};
    curr_velocity_ = _vector_3_smooth_towards(curr_velocity_, localTargetVelocity, fixed_dt, forward_accel);
    Quaternion rot = QuaternionFromMatrix(transform_);

    Vector3 worldMovement = Vector3RotateByQuaternion(curr_velocity_, rot);
    worldMovement = worldMovement + ext_gravity_velocity_*fixed_dt;

    move_global(worldMovement);

    // Rotation
    curr_roll_speed_ = _smooth_towards(curr_roll_speed_, roll*roll_speed, fixed_dt, roll_accel);
    rotate_roll(curr_roll_speed_*fixed_dt);

    ext_gravity_velocity_ = Vector3Zero();

    // Update hitbox position
    Vector3 pos = get_position();
    hitbox.min = (Vector3){pos.x - 0.2f, pos.y - 0.2f, pos.z - 0.2f};
    hitbox.max = (Vector3){pos.x + 0.2f, pos.y + 0.2f, pos.z + 0.2f};
}

void PlayerShip::after_update(Engine& engine){
    debug_.clean();
    exited_gravity = false;
    if (in_gravity && !g_flag) {
        in_gravity = false;
        exited_gravity = true;
    }

    Vector3 position = get_position();
    debug_.writeln(TextFormat("--- SHIP ---"));
    debug_.writeln(TextFormat("Position: %.2f, %.2f, %.2f", position.x, position.y, position.z));
    debug_.writeln(TextFormat("Velocity: %.2f, %.2f, %.2f", curr_velocity_.x, curr_velocity_.y, curr_velocity_.z));
    debug_.writeln(TextFormat("Roll Speed: %.2f", curr_roll_speed_));
    debug_.writeln(TextFormat("Gravity: %s", in_gravity ? "YES" : "NO"));

    Vector3 model_position = renderable_->get_position();
    debug_.writeln(TextFormat("--- SHIP MODEL ---"));
    debug_.writeln(TextFormat("Position: %.2f, %.2f, %.2f", 
                              model_position.x, model_position.y, model_position.z));
}

void PlayerShip::add_gravity(Vector3 gravityAccel) {
    ext_gravity_velocity_ += gravityAccel;
}

Planet::Planet(Vector3 p_position, float p_radius, float p_gravityRadius, float p_gravityForce)
    : radius(p_radius),
      gravity_radius(p_gravityRadius),
      gravity_force(p_gravityForce) 
{
    set_position(p_position);
}

Planet::~Planet() {}

void Planet::update(Engine& engine) {}

void Planet::fixed_update(Engine& engine) {
    auto& scene = engine.get_scene();
    PlayerShip* playerShip = (PlayerShip*)scene.get_game_object("player");
    bool colliding = CheckCollisionBoxSphere(playerShip->hitbox, get_position(), gravity_radius);
    if (colliding) {
        if (!playerShip->in_gravity){
            playerShip->entered_gravity = true;
        }
        playerShip->g_flag = true;
        playerShip->in_gravity = true;
        Vector3 direction = Vector3Normalize(Vector3Subtract(get_position(), playerShip->get_position()));
        float distance = Vector3Distance(playerShip->get_position(), get_position());
        float distanceFactor = 1.0f - Clamp(distance / gravity_radius, 0, 1);
        Vector3 gravityAccel = gravity_force * distanceFactor * direction;

        playerShip->add_gravity(gravityAccel);
    }
}

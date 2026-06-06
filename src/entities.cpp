#include "entities.hpp"

#include "scene.hpp"
#include "raylib.h"
#include "raymath.h"

float _smooth_towards(float start, float target, float speed, float threshold = 0.001f) {
    float value = Lerp(start, target, speed*GetFrameTime());
    if (std::abs(value - start) < threshold) return start;
    if (std::abs(value - target) < threshold) return target;
    return value;
}

Vector3 _vector_3_smooth_towards(Vector3 start, Vector3 target, float speed, float threshold = 0.001f) {
    Vector3 value = Vector3Lerp(start, target, speed*GetFrameTime());
    if (Vector3Distance(value, start) < threshold) return start;
    if (Vector3Distance(value, target) < threshold) return target;
    return value;
}

PlayerShip::PlayerShip() {
    hitbox = (BoundingBox){(Vector3){-0.2f, -0.2f, -0.2f}, (Vector3){0.2f, 0.2f, 0.2f}};
}

PlayerShip::~PlayerShip() {}

void PlayerShip::before_update(Scene& scene) {
    g_flag = false;
    entered_gravity = false;
}

void PlayerShip::update(Scene& scene) {
    Vector2 mouse_position = GetMousePosition();
    Vector2 mouse_distance = {mouse_position.x - 960, mouse_position.y - 540};
    float local_yaw = mouse_distance.x * -pan_speed * GetFrameTime();
    float local_pitch = mouse_distance.y * -pan_speed * GetFrameTime();

    this->rotate_pitch(local_pitch);
    this->rotate_yaw(local_yaw);

    int moveX =
        scene.is_active_input(MOVE_RIGHT) - scene.is_active_input(MOVE_LEFT);
    int moveY = scene.is_active_input(MOVE_UP) - scene.is_active_input(MOVE_DOWN);
    int moveZ =
        scene.is_active_input(MOVE_FORWARD) - scene.is_active_input(MOVE_BACK);
    int roll = 
        scene.is_active_input(ROLL_CCW) - scene.is_active_input(ROLL_CW);

    // Position Movement
    Vector3 localTargetVelocity = {moveX * right_speed, moveY * up_speed, -moveZ * forward_speed};
    curr_velocity_ = _vector_3_smooth_towards(curr_velocity_, localTargetVelocity, forward_accel);
    Quaternion rot = QuaternionFromMatrix(transform);

    Vector3 worldMovement = Vector3RotateByQuaternion(curr_velocity_, rot);
    worldMovement = worldMovement + ext_gravity_velocity_*GetFrameTime();

    move_global(worldMovement);

    // Rotation
    curr_roll_speed_ = _smooth_towards(curr_roll_speed_, roll*roll_speed, roll_accel);
    rotate_roll(curr_roll_speed_*GetFrameTime());

    ext_gravity_velocity_ = Vector3Zero();

    // Update hitbox position
    Vector3 pos = get_position();
    hitbox.min = (Vector3){pos.x - 0.2f, pos.y - 0.2f, pos.z - 0.2f};
    hitbox.max = (Vector3){pos.x + 0.2f, pos.y + 0.2f, pos.z + 0.2f};
}

void PlayerShip::after_update(Scene& scene){
    exited_gravity = false;
    if (in_gravity && !g_flag) {
        in_gravity = false;
        exited_gravity = true;
    }
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

void Planet::update(Scene& scene) {
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

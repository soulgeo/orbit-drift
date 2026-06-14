#include "camera_component.hpp"
#include "debug_component.hpp"
#include "engine.hpp"
#include "raylib.h"
#include "renderer.hpp"
#include "raymath.h"
#include "transform_component.hpp"

CameraComponent::CameraComponent(GameObject* owner, Scene* c_scene, Renderer* renderer) :
    Component(owner) 
{
    // Define Camera Profiles
    Profile default_camera;
    default_camera.target = c_scene->get_game_object("player");
    default_camera.pos_local_offset = (Vector3) {0.0f, 1.0f, -2.5f};
    default_camera.targ_local_offset = (Vector3) {0.0f, 0.0f, 3.0f};
    default_camera.fovy = 70.0f;
    camera_profiles_.push_back(default_camera);

    Profile in_gravity_camera;
    in_gravity_camera.target = c_scene->get_game_object("player");
    in_gravity_camera.pos_local_offset = (Vector3) {0.0f, 1.0f, -2.6f};
    in_gravity_camera.targ_local_offset = (Vector3) {0.0f, 0.0f, 3.0f};
    in_gravity_camera.fovy = 80.0f;
    camera_profiles_.push_back(in_gravity_camera);

    active_profile_id_ = CP_DEFAULT;

    renderer->register_camera(this);

    engine_ = owner_->get_engine();
}

CameraComponent::~CameraComponent() = default;

int CameraComponent::get_profile_id() {
    return active_profile_id_;
}

int CameraComponent::get_new_profile_id() {
    return new_profile_id_;
}

int CameraComponent::get_trans_iter() {
    return trans_iter_; 
}

Vector3 CameraComponent::get_position() {
    return transform_->get_position();
}

Vector3 CameraComponent::get_visual_position() {
    return transform_->get_visual_position();
}


int CameraComponent::get_projection() {
    return projection_;
}

float CameraComponent::get_fovy() {
    return visual_fovy_;
}

Vector3 CameraComponent::get_target() {
    return visual_target_pos_;
}

Vector3 CameraComponent::get_camera_up() {
    return visual_up_; 
}

void CameraComponent::switch_profile(int target_profile) {
    new_profile_id_ = target_profile;
    trans_iter_ = 0;
}

void CameraComponent::start() {
    transform_ = owner_->get_component<TransformComponent>();
    debug_ = owner_->get_component<DebugComponent>();

    Profile& active = camera_profiles_[active_profile_id_];
    
    // Initialize current state from active profile
    curr_state_.pos_offset = active.pos_offset;
    curr_state_.targ_offset = active.targ_offset;
    curr_state_.pos_local_offset = active.pos_local_offset;
    curr_state_.targ_local_offset = active.targ_local_offset;
    curr_state_.fovy = active.fovy;

    // Get target transform info
    GameObject& camera_target = *active.target;
    TransformComponent& camera_target_transform = camera_target.transform();

    Vector3 target_position = camera_target_transform.get_position();
    Vector3 target_forward = camera_target_transform.get_forward();
    Vector3 target_up = camera_target_transform.get_up();
    Vector3 target_right = camera_target_transform.get_right();

    // Calculate initial target position
    Vector3 right_offset = curr_state_.targ_local_offset.x * target_right;
    Vector3 up_offset = curr_state_.targ_local_offset.y * target_up;
    Vector3 forward_offset = curr_state_.targ_local_offset.z * target_forward;
    Vector3 targ_translated_offset = right_offset + up_offset + forward_offset;

    curr_target_pos_ = target_position + curr_state_.targ_offset + targ_translated_offset;
    prev_target_pos_ = curr_target_pos_;
    visual_target_pos_ = curr_target_pos_;

    // Calculate initial camera position
    right_offset = curr_state_.pos_local_offset.x * target_right;
    up_offset = curr_state_.pos_local_offset.y * target_up;
    forward_offset = curr_state_.pos_local_offset.z * target_forward;
    Vector3 pos_translated_offset = right_offset + up_offset + forward_offset;

    Vector3 world_position = target_position + curr_state_.pos_offset + pos_translated_offset;
    transform_->set_position(world_position);
    
    // Initialize Up vector
    up_ = target_up;
    prev_up_ = up_;
    visual_up_ = up_;

    // Initialize FOV
    visual_fovy_ = curr_state_.fovy;
}

void CameraComponent::fixed_update() {
    prev_target_pos_ = curr_target_pos_;
    prev_up_ = up_;

    Profile& active = camera_profiles_[active_profile_id_];

    // If new transition is starting, save the current camera state
    if (new_profile_id_ >= 0 && trans_iter_ == 0) {
        saved_state_.pos_offset = curr_state_.pos_offset;
        saved_state_.targ_offset = curr_state_.targ_offset;
        saved_state_.pos_local_offset = curr_state_.pos_local_offset;
        saved_state_.targ_local_offset = curr_state_.targ_local_offset;
        saved_state_.fovy = curr_state_.fovy;
    }

    if (new_profile_id_ < 0) {
        // Just set calced offsets to the active profile ones
        curr_state_.pos_offset = active.pos_offset;
        curr_state_.targ_offset = active.targ_offset;
        curr_state_.pos_local_offset = active.pos_local_offset;
        curr_state_.targ_local_offset = active.targ_local_offset;
        curr_state_.fovy = active.fovy;
    } else {
        // Get calced offsets for transition from saved state to new profile
        Profile& next = camera_profiles_[new_profile_id_];
        curr_state_.targ_offset = Vector3Lerp(
            saved_state_.targ_offset, 
            next.targ_offset, 
            (float)trans_iter_/max_trans_iter_
        );
        curr_state_.pos_offset = Vector3Lerp(
            saved_state_.pos_offset, 
            next.pos_offset, 
            (float)trans_iter_/max_trans_iter_
        );
        curr_state_.targ_local_offset = Vector3Lerp(
            saved_state_.targ_local_offset, 
            next.targ_local_offset, 
            (float)trans_iter_/max_trans_iter_
        );
        curr_state_.pos_local_offset  = Vector3Lerp(
            saved_state_.pos_local_offset, 
            next.pos_local_offset, 
            (float)trans_iter_/max_trans_iter_
        );
        curr_state_.fovy = Lerp(
            saved_state_.fovy, 
            next.fovy, 
            (float)trans_iter_/max_trans_iter_
        );
    }

    // Update Camera Target
    GameObject& camera_target = *active.target;
    TransformComponent& camera_target_transform = camera_target.transform();

    Vector3 target_position = camera_target_transform.get_position();
    Vector3 target_forward = camera_target_transform.get_forward();
    Vector3 target_up = camera_target_transform.get_up();
    Vector3 target_right = camera_target_transform.get_right();

    float follow_speed = 50.0f;

    // Translate local offset from local to upright space
    Vector3 targ_translated_offset;
    Vector3 right_offset = curr_state_.targ_local_offset.x * target_right;
    Vector3 up_offset = curr_state_.targ_local_offset.y * target_up;
    Vector3 forward_offset = curr_state_.targ_local_offset.z * target_forward;
    targ_translated_offset = right_offset + up_offset + forward_offset;

    // Translate to world space
    Vector3 world_target = 
        target_position + curr_state_.targ_offset + targ_translated_offset;

    curr_target_pos_ = 
        Vector3Lerp(
            curr_target_pos_, world_target, follow_speed * engine_->get_fixed_dt()
        );

    // Translate local offset from local to upright space
    Vector3 pos_translated_offset;
    right_offset = curr_state_.pos_local_offset.x * target_right;
    up_offset = curr_state_.pos_local_offset.y * target_up;
    forward_offset = curr_state_.pos_local_offset.z * target_forward;
    pos_translated_offset = right_offset + up_offset + forward_offset;

    // Translate to world space
    Vector3 world_position = 
        target_position + curr_state_.pos_offset + pos_translated_offset;

    transform_->set_position(
        Vector3Lerp(get_position(), world_position, follow_speed * engine_->get_fixed_dt())
    );

    // Update Camera Up
    up_ = target_up;

    // Manage transition loop
    if (new_profile_id_ >= 0) {
        if (trans_iter_ >= max_trans_iter_) {
            active_profile_id_ = new_profile_id_;
            new_profile_id_ = -1;
            trans_iter_ = 0;
        } else {
            trans_iter_++;
        }
    }
}

void CameraComponent::update() {
    float alpha = engine_->get_interpolation_alpha();

    visual_target_pos_ = Vector3Lerp(prev_target_pos_, curr_target_pos_, alpha);
    visual_up_ = Vector3Lerp(prev_up_, up_, alpha);
    visual_fovy_ = curr_state_.fovy; 

    Vector3 position = transform_->get_visual_position();
    if (debug_) {
        debug_->writeln(TextFormat("--- CAMERA ---"));
        debug_->writeln(TextFormat("Position: %.2f, %.2f, %.2f", 
                                   position.x, position.y, position.z));
        debug_->writeln(TextFormat("Up Vector: %.2f, %.2f, %.2f", 
                                   visual_up_.x, visual_up_.y, visual_up_.z));
        debug_->writeln(TextFormat("Active Profile: %d", active_profile_id_));
        debug_->writeln(TextFormat("Current Target Pos: %.2f, %.2f, %.2f",
                                   visual_target_pos_.x, 
                                   visual_target_pos_.y, 
                                   visual_target_pos_.z));
    }
}

// struct ShakeSetting {
//     float min;
//     float max;
//     float speed;
//     float maxFrames;
// };
//
// struct ShakeState {
//     float value;
//     int mult = 1;
//     int accum = 0;
// };
//
// struct Vector3ShakeState {
//     Vector3 value;
//     Vector3 mult = {1, 1, 1};
//     int accum = 0;
// };
//
// void _shake(ShakeState* stateRef, ShakeSetting s) {
//     auto eval = [stateRef, s]() {
//         return stateRef->value + s.speed * stateRef->mult * GetFrameTime();
//     };
//
//     float targetValue = eval();
//     if (targetValue > s.max || targetValue < s.min) {
//         stateRef->mult *= -1;
//         targetValue = eval();
//
//     } else if (stateRef->accum == s.maxFrames) {
//         auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());
//         bool flip = gen();
//
//         if (flip) {
//             stateRef->mult *= -1;
//             targetValue = eval();
//         }
//     }
//     stateRef->value = targetValue;
// }
// }

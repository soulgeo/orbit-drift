#include "camera_component.hpp"
#include "engine.hpp"
#include "renderer.hpp"
#include "raymath.h"
#include "transform_component.hpp"
#include <memory>

struct CameraComponent::Impl {
    std::vector<Profile> camera_profiles;
    State curr_state;
    State saved_state;
    
    Vector3 curr_target_pos;
    Vector3 prev_target_pos;
    Vector3 visual_target_pos;

    Vector3 up;
    Vector3 prev_up;
    Vector3 visual_up;

    float visual_fovy;

    int projection = CAMERA_PERSPECTIVE;
    int active_profile_id = CP_DEFAULT;
    int new_profile_id = -1;
    int trans_iter = 0;
    int max_trans_iter = 40;

    TransformComponent* cached_transform;

    Engine* engine;
};

CameraComponent::CameraComponent(GameObject* owner, Scene* c_scene, Renderer* renderer) :
    Component(owner) 
{
    impl_ = std::make_unique<Impl>();

    // Define Camera Profiles
    Profile defaultCamera;
    defaultCamera.target = c_scene->get_game_object("player");
    defaultCamera.pos_local_offset = (Vector3) {0.0f, 1.0f, -2.5f};
    defaultCamera.targ_local_offset = (Vector3) {0.0f, 0.0f, 3.0f};
    defaultCamera.fovy = 70.0f;
    impl_->camera_profiles.push_back(defaultCamera);

    Profile inGravityCamera;
    inGravityCamera.target = c_scene->get_game_object("player");
    inGravityCamera.pos_local_offset = (Vector3) {0.0f, 1.0f, -2.8f};
    inGravityCamera.targ_local_offset = (Vector3) {0.0f, 0.0f, 3.0f};
    inGravityCamera.fovy = 80.0f;
    impl_->camera_profiles.push_back(inGravityCamera);

    impl_->active_profile_id = CP_DEFAULT;

    renderer->register_camera(this);

    impl_->engine = owner_->get_engine();
}

CameraComponent::~CameraComponent() = default;

int CameraComponent::get_profile_id() {
    return impl_->active_profile_id;
}

int CameraComponent::get_new_profile_id() {
    return impl_->new_profile_id;
}

int CameraComponent::get_trans_iter() {
    return impl_->trans_iter; 
}

Vector3 CameraComponent::get_position() {
    return impl_->cached_transform->get_position();
}

int CameraComponent::get_projection() {
    return impl_->projection;
}

float CameraComponent::get_fovy() {
    return impl_->visual_fovy;
}

Vector3 CameraComponent::get_target() {
    return impl_->visual_target_pos;
}

Vector3 CameraComponent::get_camera_up() {
    return impl_->visual_up; 
}

void CameraComponent::switch_profile(int target_profile) {
    impl_->new_profile_id = target_profile;
    impl_->trans_iter = 0;
}

void CameraComponent::start() {
    impl_->cached_transform = owner_->get_component<TransformComponent>();
}

void CameraComponent::fixed_update() {
    impl_->prev_target_pos = impl_->curr_target_pos;
    impl_->prev_up = impl_->up;

    Profile& active = impl_->camera_profiles[impl_->active_profile_id];

    // If new transition is starting, save the current camera state
    if (impl_->new_profile_id >= 0 && impl_->trans_iter == 0) {
        impl_->saved_state.pos_offset = impl_->curr_state.pos_offset;
        impl_->saved_state.targ_offset = impl_->curr_state.targ_offset;
        impl_->saved_state.pos_local_offset = impl_->curr_state.pos_local_offset;
        impl_->saved_state.targ_local_offset = impl_->curr_state.targ_local_offset;
        impl_->saved_state.fovy = impl_->curr_state.fovy;
    }

    if (impl_->new_profile_id < 0) {
        // Just set calced offsets to the active profile ones
        impl_->curr_state.pos_offset = active.pos_offset;
        impl_->curr_state.targ_offset = active.targ_offset;
        impl_->curr_state.pos_local_offset = active.pos_local_offset;
        impl_->curr_state.targ_local_offset = active.targ_local_offset;
        impl_->curr_state.fovy = active.fovy;
    } else {
        // Get calced offsets for transition from saved state to new profile
        Profile& next = impl_->camera_profiles[impl_->new_profile_id];
        impl_->curr_state.targ_offset = Vector3Lerp(
            impl_->saved_state.targ_offset, 
            next.targ_offset, 
            (float)impl_->trans_iter/impl_->max_trans_iter
        );
        impl_->curr_state.pos_offset = Vector3Lerp(
            impl_->saved_state.pos_offset, 
            next.pos_offset, 
            (float)impl_->trans_iter/impl_->max_trans_iter
        );
        impl_->curr_state.targ_local_offset = Vector3Lerp(
            impl_->saved_state.targ_local_offset, 
            next.targ_local_offset, 
            (float)impl_->trans_iter/impl_->max_trans_iter
        );
        impl_->curr_state.pos_local_offset  = Vector3Lerp(
            impl_->saved_state.pos_local_offset, 
            next.pos_local_offset, 
            (float)impl_->trans_iter/impl_->max_trans_iter
        );
        impl_->curr_state.fovy = Lerp(
            impl_->saved_state.fovy, 
            next.fovy, 
            (float)impl_->trans_iter/impl_->max_trans_iter
        );
    }

    // Update Camera Target
    GameObject& camera_target = *active.target;
    TransformComponent& camera_target_transform = camera_target.transform();

    Vector3 target_position = camera_target_transform.get_position();
    Vector3 target_forward = camera_target_transform.get_forward();
    Vector3 target_up = camera_target_transform.get_up();
    Vector3 target_right = camera_target_transform.get_right();

    float follow_speed = 30.0f;

    // Translate local offset from local to upright space
    Vector3 targ_translated_offset;
    Vector3 right_offset = impl_->curr_state.targ_local_offset.x * target_right;
    Vector3 up_offset = impl_->curr_state.targ_local_offset.y * target_up;
    Vector3 forward_offset = impl_->curr_state.targ_local_offset.z * target_forward;
    targ_translated_offset = right_offset + up_offset + forward_offset;

    // Translate to world space
    Vector3 world_target = 
        target_position + impl_->curr_state.targ_offset + targ_translated_offset;

    impl_->curr_target_pos = 
        Vector3Lerp(
            impl_->curr_target_pos, world_target, follow_speed * impl_->engine->get_fixed_dt()
        );

    // Translate local offset from local to upright space
    Vector3 pos_translated_offset;
    right_offset = impl_->curr_state.pos_local_offset.x * target_right;
    up_offset = impl_->curr_state.pos_local_offset.y * target_up;
    forward_offset = impl_->curr_state.pos_local_offset.z * target_forward;
    pos_translated_offset = right_offset + up_offset + forward_offset;

    // Translate to world space
    Vector3 world_position = 
        target_position + impl_->curr_state.pos_offset + pos_translated_offset;

    impl_->cached_transform->set_position(
        Vector3Lerp(get_position(), world_position, follow_speed * impl_->engine->get_fixed_dt())
    );

    // Update Camera Up
    impl_->up = target_up;

    // Manage transition loop
    if (impl_->new_profile_id >= 0) {
        if (impl_->trans_iter >= impl_->max_trans_iter) {
            impl_->active_profile_id = impl_->new_profile_id;
            impl_->new_profile_id = -1;
            impl_->trans_iter = 0;
        } else {
            impl_->trans_iter++;
        }
    }
}

void CameraComponent::update() {
    float alpha = impl_->engine->get_interpolation_alpha();

    impl_->visual_target_pos = Vector3Lerp(impl_->prev_target_pos, impl_->curr_target_pos, alpha);
    impl_->visual_up = Vector3Lerp(impl_->prev_up, impl_->up, alpha);
    impl_->visual_fovy = impl_->curr_state.fovy; 
}

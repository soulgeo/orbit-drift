#include "camera.hpp"
#include "engine.hpp"

struct CameraBodyImpl {
    std::vector<CameraProfile> camera_profiles;
};

CameraBody::CameraBody(Scene* c_scene, Renderer* renderer) {
    impl_ = new CameraBodyImpl;

    // Define Camera Profiles
    CameraProfile defaultCamera;
    defaultCamera.target = c_scene->get_game_object("player");
    defaultCamera.pos_local_offset = (Vector3) {0.0f, 1.0f, -2.5f};
    defaultCamera.targ_local_offset = (Vector3) {0.0f, 0.0f, 3.0f};
    defaultCamera.fovy = 60.0f;
    impl_->camera_profiles.push_back(defaultCamera);

    CameraProfile inGravityCamera;
    inGravityCamera.target = c_scene->get_game_object("player");
    inGravityCamera.pos_local_offset = (Vector3) {0.0f, 1.0f, -3.8f};
    inGravityCamera.targ_local_offset = (Vector3) {0.0f, 0.0f, 3.0f};
    inGravityCamera.fovy = 80.0f;
    impl_->camera_profiles.push_back(inGravityCamera);

    active_profile_id_ = CP_DEFAULT;

    renderer->set_camera_body(this);
}

void CameraBody::fixed_update(Engine& engine) {
    prev_target_pos_ = curr_target_pos_;
    prev_up_ = up_;

    CameraProfile& active = impl_->camera_profiles[active_profile_id_];

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
        CameraProfile& next = impl_->camera_profiles[new_profile_id_];
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

    Vector3 target_position = camera_target.get_position();
    Vector3 target_forward = camera_target.get_forward();
    Vector3 target_up = camera_target.get_up();
    Vector3 target_right = camera_target.get_right();

    float follow_speed = 30.0f;

    // Translate local offset from local to upright space
    Vector3 targ_translated_offset;
    Vector3 right_offset = curr_state_.targ_local_offset.x * target_right;
    Vector3 up_offset = curr_state_.targ_local_offset.y * target_up;
    Vector3 forward_offset = curr_state_.targ_local_offset.z * target_forward;
    targ_translated_offset = right_offset + up_offset + forward_offset;

    // Finally translate to world space
    Vector3 world_target = target_position + curr_state_.targ_offset + targ_translated_offset;

    curr_target_pos_ = Vector3Lerp(curr_target_pos_, world_target, follow_speed*engine.get_fixed_dt());

    // Translate local offset from local to upright space
    Vector3 pos_translated_offset;
    right_offset = curr_state_.pos_local_offset.x * target_right;
    up_offset = curr_state_.pos_local_offset.y * target_up;
    forward_offset = curr_state_.pos_local_offset.z * target_forward;
    pos_translated_offset = right_offset + up_offset + forward_offset;

    // Finally translate to world space
    Vector3 world_position = target_position + curr_state_.pos_offset + pos_translated_offset;

    set_position(Vector3Lerp(get_position(), world_position, follow_speed*engine.get_fixed_dt()));

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

void CameraBody::update(Engine& engine) {
    float alpha = engine.get_interpolation_alpha();

    visual_target_pos_ = Vector3Lerp(prev_target_pos_, curr_target_pos_, alpha);
    visual_up_ = Vector3Lerp(prev_up_, up_, alpha);
    visual_fovy_ = curr_state_.fovy; 
}

#include "sputnik/rendering/camera_component.hpp"
#include "events/event_dispatcher.hpp"
#include "sputnik/rendering/debug_component.hpp"
#include "sputnik/core/engine.hpp"
#include <raylib.h>
#include "sputnik/rendering/renderer.hpp"
#include <raymath.h>
#include <string>
#include <string_view>
#include "sputnik/ecs/transform_component.hpp"

namespace Sputnik {

    CameraComponent::CameraComponent(GameObject* owner, Scene* c_scene, Renderer* renderer, EventDispatcher* event_dsp) :
        Component(owner), projection_(CAMERA_PERSPECTIVE), follow_speed_(100.0f) 
    {
        // Define Default Profile
        Profile default_profile;
        default_profile.target_transform = nullptr;
        default_profile.pos_local_offset = (Vector3) {0.0f, 1.0f, -2.5f};
        default_profile.targ_local_offset = (Vector3) {0.0f, 0.0f, 3.0f};
        default_profile.fovy = 70.0f;
        profiles_["default"] = default_profile;
        active_profile_ = &profiles_["default"];
        active_profile_name_ = "default";
        next_profile_name_ = "";

        renderer->register_camera(this);
        event_dsp->add_subscriber(this);

        engine_ = owner_->engine();
    }

    CameraComponent::~CameraComponent() = default;

    CameraComponent::Profile* CameraComponent::active_profile() {
        return active_profile_;
    }

    int CameraComponent::trans_iter() {
        return trans_iter_; 
    }

    Vector3 CameraComponent::position() {
        return transform_->position();
    }

    Vector3 CameraComponent::visual_position() {
        return transform_->visual_position();
    }


    int CameraComponent::projection() {
        return projection_;
    }

    float CameraComponent::fovy() {
        return visual_fovy_;
    }

    Vector3 CameraComponent::target() {
        return visual_target_pos_;
    }

    Vector3 CameraComponent::camera_up() {
        return visual_up_; 
    }

    void CameraComponent::add_profile(
        std::string_view name, CameraComponent::Profile profile) 
    {
        profiles_[std::string(name)] = profile;
    }

    void CameraComponent::set_active_profile(std::string_view next_profile_name) {
        auto it = profiles_.find(std::string(next_profile_name));
        if (it == profiles_.end()) { return; }
        active_profile_ = &it->second;
        active_profile_name_ = std::string(next_profile_name);
    }

    void CameraComponent::switch_profile(std::string_view next_profile_name) {
        auto it = profiles_.find(std::string(next_profile_name));
        if (it == profiles_.end() || &it->second == active_profile_) { return; }
        next_profile_ = &it->second;
        next_profile_name_ = std::string(next_profile_name);
        trans_iter_ = 0;
    }

    void CameraComponent::assign_profile_to_event(
        std::string_view event_name, std::string_view profile_name)
    {
        event_to_profile_map_[std::string(event_name)] = std::string(profile_name);
    }

    void CameraComponent::on_event(Event event) {
        auto it = event_to_profile_map_.find(event.message);
        if (it == event_to_profile_map_.end()) { return; }
        switch_profile(it->second);
    }

    void CameraComponent::init() {
        transform_ = owner_->component<TransformComponent>();
        debug_ = owner_->component<DebugComponent>();

        Profile& active = *active_profile_;

        Vector3 target_position = Vector3Zero();
        Vector3 target_right = {1.0f, 0.0f, 0.0f};
        Vector3 target_up = {0.0f, 1.0f, 0.0f};
        Vector3 target_forward = {0.0f, 0.0f, 1.0f};
        if (active.target_transform) {
            target_position = active.target_transform->position();
            target_up = active.target_transform->up();
            target_right = active.target_transform->right();
            target_forward = active.target_transform->forward();
        }
        
        // Initialize current state from active profile
        curr_state_.target = target_position;
        curr_state_.pos_upright_offset = active.pos_upright_offset;
        curr_state_.targ_upright_offset = active.targ_upright_offset;
        curr_state_.pos_local_offset = active.pos_local_offset;
        curr_state_.targ_local_offset = active.targ_local_offset;
        curr_state_.fovy = active.fovy;

        // Calculate initial target position
        Vector3 right_offset = curr_state_.targ_local_offset.x * target_right;
        Vector3 up_offset = curr_state_.targ_local_offset.y * target_up;
        Vector3 forward_offset = curr_state_.targ_local_offset.z * target_forward;
        Vector3 targ_translated_offset = right_offset + up_offset + forward_offset;

        curr_target_pos_ = target_position + curr_state_.targ_upright_offset + targ_translated_offset;
        prev_target_pos_ = curr_target_pos_;
        visual_target_pos_ = curr_target_pos_;

        // Calculate initial camera position
        right_offset = curr_state_.pos_local_offset.x * target_right;
        up_offset = curr_state_.pos_local_offset.y * target_up;
        forward_offset = curr_state_.pos_local_offset.z * target_forward;
        Vector3 pos_translated_offset = right_offset + up_offset + forward_offset;

        Vector3 world_position = target_position + curr_state_.pos_upright_offset + pos_translated_offset;
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

        Profile& active = *active_profile_;

        Vector3 target_position = Vector3Zero();
        Vector3 target_right = {1.0f, 0.0f, 0.0f};
        Vector3 target_up = {0.0f, 1.0f, 0.0f};
        Vector3 target_forward = {0.0f, 0.0f, 1.0f};
        if (active.target_transform) {
            target_position = active.target_transform->position();
            target_forward = active.target_transform->forward();
            target_up = active.target_transform->up();
            target_right = active.target_transform->right();
        }

        // If a profile transition is starting, save the current profile state
        if (next_profile_.has_value() && trans_iter_ == 0) {
            saved_state_.target = curr_state_.target;
            saved_state_.pos_upright_offset = curr_state_.pos_upright_offset;
            saved_state_.targ_upright_offset = curr_state_.targ_upright_offset;
            saved_state_.pos_local_offset = curr_state_.pos_local_offset;
            saved_state_.targ_local_offset = curr_state_.targ_local_offset;
            saved_state_.fovy = curr_state_.fovy;
        }

        // --- Current profile state calculation ---
        if (!next_profile_.has_value()) {
            // Just set current profile state to the active profile
            curr_state_.target = target_position;
            curr_state_.pos_upright_offset = active.pos_upright_offset;
            curr_state_.targ_upright_offset = active.targ_upright_offset;
            curr_state_.pos_local_offset = active.pos_local_offset;
            curr_state_.targ_local_offset = active.targ_local_offset;
            curr_state_.fovy = active.fovy;
        } else {
            // Profile transition: calc intermediate state, set current state equal to it
            Profile& next = *next_profile_.value();
            Vector3 next_target_position = Vector3Zero();
            bool new_target = false;
            if (next.target_transform) {
                next_target_position = next.target_transform->position();
                if (active.target_transform != next.target_transform){
                    new_target = true;
                }
            }
            curr_state_.target = target_position;
            if (new_target){
                curr_state_.target = Vector3Lerp(
                    saved_state_.target, 
                    next_target_position, 
                    (float)trans_iter_/max_trans_iter_
                );
            }
            curr_state_.targ_upright_offset = Vector3Lerp(
                saved_state_.targ_upright_offset, 
                next.targ_upright_offset, 
                (float)trans_iter_/max_trans_iter_
            );
            curr_state_.pos_upright_offset = Vector3Lerp(
                saved_state_.pos_upright_offset, 
                next.pos_upright_offset, 
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

        // Translate local offset from local to upright space
        Vector3 targ_translated_offset;
        Vector3 right_offset = curr_state_.targ_local_offset.x * target_right;
        Vector3 up_offset = curr_state_.targ_local_offset.y * target_up;
        Vector3 forward_offset = curr_state_.targ_local_offset.z * target_forward;
        targ_translated_offset = right_offset + up_offset + forward_offset;

        // Translate to world space
        Vector3 world_target = 
            curr_state_.target + curr_state_.targ_upright_offset + targ_translated_offset;

        curr_target_pos_ = 
            Vector3Lerp(
                curr_target_pos_, world_target, follow_speed_ * engine_->fixed_dt()
            );

        // Translate local offset from local to upright space
        Vector3 pos_translated_offset;
        right_offset = curr_state_.pos_local_offset.x * target_right;
        up_offset = curr_state_.pos_local_offset.y * target_up;
        forward_offset = curr_state_.pos_local_offset.z * target_forward;
        pos_translated_offset = right_offset + up_offset + forward_offset;

        // Translate to world space
        Vector3 world_position = 
            curr_state_.target + curr_state_.pos_upright_offset + pos_translated_offset;

        transform_->set_position(
            Vector3Lerp(position(), world_position, follow_speed_ * engine_->fixed_dt())
        );

        // Update Camera Up
        up_ = target_up;

        // Manage transition loop
        if (next_profile_.has_value()) {
            if (trans_iter_ >= max_trans_iter_) {
                active_profile_ = next_profile_.value();
                active_profile_name_ = next_profile_name_;
                next_profile_.reset();
                next_profile_name_ = "";
                trans_iter_ = 0;
            } else {
                trans_iter_++;
            }
        }
    }

    void CameraComponent::update() {
        float alpha = engine_->interpolation_alpha();

        visual_target_pos_ = Vector3Lerp(prev_target_pos_, curr_target_pos_, alpha);
        visual_up_ = Vector3Lerp(prev_up_, up_, alpha);
        visual_fovy_ = curr_state_.fovy; 

        Vector3 position = transform_->visual_position();
        if (debug_) {
            debug_->writeln(TextFormat("--- CAMERA ---"));
            debug_->writeln(TextFormat("Position: %.2f, %.2f, %.2f", 
                                       position.x, position.y, position.z));
            debug_->writeln(TextFormat("Up Vector: %.2f, %.2f, %.2f", 
                                       visual_up_.x, visual_up_.y, visual_up_.z));
            debug_->writeln(TextFormat("Current Target Pos: %.2f, %.2f, %.2f",
                                       visual_target_pos_.x, 
                                       visual_target_pos_.y, 
                                       visual_target_pos_.z));
            debug_->writeln(TextFormat("Active Profile: %s", active_profile_name_.c_str()));
            debug_->writeln(TextFormat("Next Profile: %s", next_profile_name_.c_str()));
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

}

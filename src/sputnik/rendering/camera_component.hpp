#pragma once

#include "ecs/transform_component.hpp"
#include "events/event_dispatcher.hpp"
#include "sputnik/ecs/component.hpp"
#include "sputnik/ecs/game_object.hpp"
#include <optional>
#include <raylib.h>
#include "sputnik/ecs/scene.hpp"
#include <string_view>
#include <unordered_map>

namespace Sputnik {

    class CameraComponent : public Component {
    public:
        struct Profile {
            TransformComponent* target_transform;
            Vector3 pos_upright_offset;
            Vector3 targ_upright_offset;
            Vector3 pos_local_offset;
            Vector3 targ_local_offset;
            float fovy;
        };

    private:
        struct ProfileState {
            Vector3 target;
            Vector3 pos_upright_offset;
            Vector3 targ_upright_offset;
            Vector3 pos_local_offset;
            Vector3 targ_local_offset;
            float fovy;
        };

        float follow_speed_;

        std::unordered_map<std::string, Profile> profiles_;
        std::unordered_map<std::string, std::string> event_to_profile_map_;
        ProfileState curr_state_;
        ProfileState saved_state_;
        
        Vector3 curr_target_pos_;
        Vector3 prev_target_pos_;
        Vector3 visual_target_pos_;

        Vector3 up_;
        Vector3 prev_up_;
        Vector3 visual_up_;

        float visual_fovy_;

        int projection_;
        Profile* active_profile_;
        std::string active_profile_name_;
        std::optional<Profile*> next_profile_;
        std::string next_profile_name_;
        int trans_iter_ = 0;
        int max_trans_iter_ = 30;

        TransformComponent* transform_;
        class DebugComponent* debug_;
        class Engine* engine_;

    public:
        CameraComponent(
            GameObject* owner, Scene* scene, 
            Renderer* renderer, EventDispatcher* event_dsp
        );
        ~CameraComponent();

        void init() override;
        void fixed_update() override;
        void update() override;

        void on_event(Event event) override;

        Profile* active_profile();
        int trans_iter();
    
        Vector3 position();
        Vector3 visual_position();
        int projection();
        float fovy();
        Vector3 target();
        Vector3 camera_up();

        float follow_speed() const;
        void set_follow_speed(float follow_speed);

        void add_profile(std::string_view profile_name, Profile profile);
        void set_active_profile(std::string_view profile_name);
        void switch_profile(std::string_view next_profile_name);

        void assign_profile_to_event(
            std::string_view event_name, std::string_view profile_name
        );
    };

}

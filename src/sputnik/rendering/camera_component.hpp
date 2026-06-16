#ifndef CAMERA_COMPONENT_HPP
#define CAMERA_COMPONENT_HPP

#include "sputnik/ecs/component.hpp"
#include "sputnik/ecs/game_object.hpp"
#include <raylib.h>
#include "sputnik/ecs/scene.hpp"
#include <vector>

namespace Sputnik {

    class CameraComponent : public Component {
        struct Profile {
            GameObject* target;
            Vector3 pos_offset = (Vector3) {0.0f, 0.0f, 0.0f};
            Vector3 targ_offset = (Vector3) {0.0f, 0.0f, 0.0f};
            Vector3 pos_local_offset = (Vector3) {0.0f, 1.0f, -2.5f};
            Vector3 targ_local_offset = (Vector3) {0.0f, 0.0f, 3.0f};
            float fovy;
        };

        struct State {
            Vector3 pos_offset;
            Vector3 targ_offset;
            Vector3 pos_local_offset;
            Vector3 targ_local_offset;
            float fovy;
        };

        std::vector<Profile> camera_profiles_;
        State curr_state_;
        State saved_state_;
        
        Vector3 curr_target_pos_;
        Vector3 prev_target_pos_;
        Vector3 visual_target_pos_;

        Vector3 up_;
        Vector3 prev_up_;
        Vector3 visual_up_;

        float visual_fovy_;

        int projection_ = CAMERA_PERSPECTIVE;
        int active_profile_id_ = CP_DEFAULT;
        int new_profile_id_ = -1;
        int trans_iter_ = 0;
        int max_trans_iter_ = 30;

        class TransformComponent* transform_;
        class DebugComponent* debug_;
        class Engine* engine_;

    public:
        CameraComponent(GameObject* owner, Scene* scene, Renderer* renderer);
        ~CameraComponent();

        enum ProfileId {
            CP_DEFAULT = 0,
            CP_IN_GRAVITY,
        };

        int profile_id();
        int new_profile_id();
        int trans_iter();
        Vector3 position();
        Vector3 visual_position();
        int projection();
        float fovy();
        Vector3 target();
        Vector3 camera_up();

        void switch_profile(int target_profile);

        void start() override;

        void fixed_update() override;
        void update() override;
    };

}

#endif // !CAMERA_COMPONENT_HPP

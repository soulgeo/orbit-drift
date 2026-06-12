#ifndef CAMERA_COMPONENT_HPP
#define CAMERA_COMPONENT_HPP

#include "component.hpp"
#include "game_object.hpp"
#include "raylib.h"
#include "scene.hpp"
#include <memory>

class CameraComponent : public Component {
    struct Impl;
    std::unique_ptr<Impl> impl_;

    struct Profile {
        GameObject* target;
        Vector3 pos_offset = (Vector3) {0.0f, 0.0f, 0.0f};
        Vector3 targ_offset = (Vector3) {0.0f, 0.0f, 0.0f};
        Vector3 pos_local_offset = (Vector3) {0.0f, 0.0f, 0.0f};
        Vector3 targ_local_offset = (Vector3) {0.0f, 0.0f, 0.0f};
        float fovy;
    };

    struct State {
        Vector3 pos_offset;
        Vector3 targ_offset;
        Vector3 pos_local_offset;
        Vector3 targ_local_offset;
        float fovy;
    };

    enum ProfileId {
        CP_DEFAULT = 0,
        CP_IN_GRAVITY,
    };

public:
    CameraComponent(GameObject* owner, Scene* scene, Renderer* renderer);
    ~CameraComponent();

    int get_profile_id();
    int get_new_profile_id();
    int get_trans_iter();
    Vector3 get_position();
    int get_projection();
    float get_fovy();
    Vector3 get_target();
    Vector3 get_camera_up();

    void switch_profile(int target_profile);

    void start() override;

    void fixed_update() override;
    void update() override;
};

#endif // !CAMERA_COMPONENT_HPP

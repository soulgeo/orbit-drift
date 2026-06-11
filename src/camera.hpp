#include "game_object.hpp"
#include "raylib.h"
#include "scene.hpp"
#include <memory>

struct CameraProfile {
    GameObject* target;
    Vector3 pos_offset = (Vector3) {0.0f, 0.0f, 0.0f};
    Vector3 targ_offset = (Vector3) {0.0f, 0.0f, 0.0f};
    Vector3 pos_local_offset = (Vector3) {0.0f, 0.0f, 0.0f};
    Vector3 targ_local_offset = (Vector3) {0.0f, 0.0f, 0.0f};
    float fovy;
};

struct CameraState {
    Vector3 pos_offset;
    Vector3 targ_offset;
    Vector3 pos_local_offset;
    Vector3 targ_local_offset;
    float fovy;
};

enum CameraProfileId {
    CP_DEFAULT = 0,
    CP_IN_GRAVITY,
};


class CameraBody : public GameObject{
public:
    CameraBody(Scene* scene, Renderer* renderer);
    ~CameraBody();

    int get_profile_id();
    int get_new_profile_id();
    int get_trans_iter();
    int get_projection();
    float get_fovy();
    Vector3 get_target();
    Vector3 get_camera_up();

    void switch_profile(int target_profile);

    void fixed_update(Engine& engine) override;
    void update(Engine& engine) override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

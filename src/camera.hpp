#include "game_object.hpp"
#include "raylib.h"
#include "scene.hpp"

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

struct CameraBodyImpl;

class CameraBody : public GameObject{
public:
    CameraBody(Scene* scene, Renderer* renderer);
    ~CameraBody() {};

    int get_profile_id() { return active_profile_id_; }
    int get_new_profile_id() { return new_profile_id_; }
    int get_trans_iter() { return trans_iter_; }
    int get_projection() { return projection_; }
    int get_fovy() { return fovy_; }
    Vector3 get_target() { return curr_target_pos_; }
    Vector3 get_camera_up() { return up_; }


    void switch_profile(int target_profile){
        new_profile_id_ = target_profile;
        trans_iter_ = 0;
    };

    void fixed_update(Engine& engine) override;

private:
    CameraState curr_state_;
    CameraState saved_state_;
    CameraBodyImpl* impl_;
    Vector3 curr_target_pos_;
    Vector3 up_;
    int fovy_;
    int projection_ = CAMERA_PERSPECTIVE;
    int active_profile_id_ = CP_DEFAULT;
    int new_profile_id_ = -1;
    int trans_iter_ = 0;
    int max_trans_iter_ = 16;
};

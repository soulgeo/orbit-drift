#ifndef RENDER_HPP
#define RENDER_HPP

#include "scene.hpp"
#include "game_object.hpp"

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

struct CameraManagerImpl;

class CameraManager {
public:
    CameraManager(Scene* scene);
    ~CameraManager() {};

    Camera camera = {0};

    int get_profile_id() {
        return active_profile_id_;
    }
    
    int get_new_profile_id() {
        return new_profile_id_;
    }

    int get_trans_iter() {
        return trans_iter_;
    }

    void switch_profile(int target_profile){
        new_profile_id_ = target_profile;
        trans_iter_ = 0;
    };

    void update();


private:
    CameraState curr_state_;
    CameraState saved_state_;
    CameraManagerImpl* impl_;
    Scene* scene_;
    int active_profile_id_ = CP_DEFAULT;
    int new_profile_id_ = -1;
    int trans_iter_ = 0;
    int max_trans_iter_ = 16;
};

struct RendererImpl;

class Renderer {
public:
    Renderer(Scene* scene);
    ~Renderer();

    CameraManager cam_manager;

    void update();
    void draw_3d();
    void draw_ui();

private:
    Scene* scene_;
    Shader fog_;
    RendererImpl* impl_;

    float dt_;
};

#endif // !RENDER_HPP

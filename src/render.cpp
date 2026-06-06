#include "render.hpp"

#include <random>
#include <string>
#include <tuple>
#include <unordered_map>

#include "entities.hpp"
#include "game_object.hpp"
#include "raylib.h"
#include "raymath.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else
#define GLSL_VERSION 100
#endif

struct CameraManagerImpl {
    std::vector<CameraProfile> camera_profiles;
};

struct RendererImpl {
    std::unordered_map<std::string, Model> models;
    std::unordered_map<std::string, std::tuple<Model, Matrix, Color>>
    obj_model_refs;
};

CameraManager::CameraManager(Scene* c_scene) : scene_(c_scene) {
    impl_ = new CameraManagerImpl;

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

    // Init camera
    active_profile_id_ = CP_DEFAULT;
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

}

void CameraManager::update() {
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

    Vector3 targ_translated_offset;
    // Translate local offset from local to upright space
    Vector3 right_offset = curr_state_.targ_local_offset.x * target_right;
    Vector3 up_offset = curr_state_.targ_local_offset.y * target_up;
    Vector3 forward_offset = curr_state_.targ_local_offset.z * target_forward;
    targ_translated_offset = right_offset + up_offset + forward_offset;

    // Finally translate to world space
    Vector3 world_target = target_position + curr_state_.targ_offset + targ_translated_offset;

    camera.target = Vector3Lerp(camera.target, world_target, follow_speed*GetFrameTime());

    Vector3 pos_translated_offset;
    // Translate local offset from local to upright space
    right_offset = curr_state_.pos_local_offset.x * target_right;
    up_offset = curr_state_.pos_local_offset.y * target_up;
    forward_offset = curr_state_.pos_local_offset.z * target_forward;
    pos_translated_offset = right_offset + up_offset + forward_offset;

    // Finally translate to world space
    Vector3 world_position = target_position + curr_state_.pos_offset + pos_translated_offset;

    camera.position = Vector3Lerp(camera.position, world_position, follow_speed*GetFrameTime());

    // Update Camera Up
    camera.up = target_up;

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

Renderer::Renderer(Scene *r_scene) : scene_(r_scene), cam_manager(r_scene) {
    impl_ = new RendererImpl;

    // Init models
    impl_->models["player_model"] = LoadModel("resources/models/scene.gltf");
    // impl_->models["player_model"] = LoadModelFromMesh(GenMeshCone(0.2f,
    // 0.5f, 16.0f));
    Texture2D texture =
        LoadTexture("resources/models/textures/Material_baseColor.png");
    impl_->models["player_model"]
        .materials[0]
        .maps[MATERIAL_MAP_DIFFUSE]
        .texture = texture;

    impl_->models["planet_model"] =
        LoadModelFromMesh(GenMeshSphere(1.0f, 30.0f, 30.0f));

    impl_->obj_model_refs["player"] = {
        impl_->models["player_model"],
        MatrixRotateX(180.0f * DEG2RAD) * 
            MatrixRotateZ(180.0f * DEG2RAD) * 
            MatrixScale(0.2f, 0.2f, 0.2f),
        MAGENTA};

    float scalar;

    scalar = ((Planet *)r_scene->get_game_object("planet1"))->radius;
    impl_->obj_model_refs["planet1"] = {impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar),
        PURPLE};

    scalar = ((Planet *)r_scene->get_game_object("planet2"))->radius;
    impl_->obj_model_refs["planet2"] = {impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar), GREEN};

    scalar = ((Planet *)r_scene->get_game_object("planet3"))->radius;
    impl_->obj_model_refs["planet3"] = {impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar),
        YELLOW};

    scalar = ((Planet *)r_scene->get_game_object("planet4"))->radius;
    impl_->obj_model_refs["planet4"] = {impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar), BLUE};

    // Init shaders
    fog_ = LoadShader(TextFormat("resources/shaders/ambient.vert", GLSL_VERSION),
                     TextFormat("resources/shaders/ambient.frag", GLSL_VERSION));

    for (auto &[name, tuple] : impl_->obj_model_refs) {
        std::get<0>(tuple).materials[0].shader = fog_;
    }
}

Renderer::~Renderer() {
    for (auto &[name, model] : impl_->models) {
        UnloadModel(model);
    }
    UnloadShader(fog_);
    delete impl_;
}


void Renderer::update() {
    dt_ = GetFrameTime();

    int camProfile = cam_manager.get_profile_id(); 
    auto player = (PlayerShip *)scene_->get_game_object("player");
    if (player->entered_gravity && camProfile == CP_DEFAULT) {
        cam_manager.switch_profile(CP_IN_GRAVITY);
    }
    if (player->exited_gravity && camProfile == CP_IN_GRAVITY) {
        cam_manager.switch_profile(CP_DEFAULT);
    }

    cam_manager.update();

    // Update model transforms
    for (auto &[name, tuple] : impl_->obj_model_refs) {
        GameObject *object = scene_->get_game_object(name);
        if (object) {
            std::get<0>(tuple).transform =
                std::get<1>(tuple) * object->transform;
        }
    }
}

void Renderer::draw_3d() {
    int distLoc = GetShaderLocation(fog_, "viewPos");
    SetShaderValue(fog_, distLoc, &cam_manager.camera.position, SHADER_UNIFORM_VEC3);
    SetShaderValue(fog_, fog_.locs[SHADER_LOC_VECTOR_VIEW], &cam_manager.camera.position,
                   SHADER_UNIFORM_VEC3);

    for (auto &[name, tuple] : impl_->obj_model_refs) {
        Model &model = std::get<0>(tuple);
        DrawModel(model, Vector3Zero(), 1.0, std::get<2>(tuple));
        // DrawModelWires(model, Vector3Zero(), 1.0f,
        //                ColorTint(std::get<2>(tuple), (Color){200, 200, 200,
        //                255}));
    }
}

void Renderer::draw_ui() {
    // auto text = "CAM PROFILE: " + std::to_string(cam_manager.get_profile_id());
    // DrawText(text.c_str(), 30, 50, 20, YELLOW);
    // text = "CAM TRANSITION PROFILE: " + std::to_string(cam_manager.get_new_profile_id());
    // DrawText(text.c_str(), 30, 75, 20, YELLOW);
    // text = "CAM TRANSITION ITERATOR: " + std::to_string(cam_manager.get_trans_iter());
    // DrawText(text.c_str(), 30, 100, 20, YELLOW);
}

struct ShakeSetting {
    float min;
    float max;
    float speed;
    float maxFrames;
};

struct ShakeState {
    float value;
    int mult = 1;
    int accum = 0;
};

struct Vector3ShakeState {
    Vector3 value;
    Vector3 mult = {1, 1, 1};
    int accum = 0;
};

void _shake(ShakeState* stateRef, ShakeSetting s) {
    auto eval = [stateRef, s]() {
        return stateRef->value + s.speed * stateRef->mult * GetFrameTime();
    };

    float targetValue = eval();
    if (targetValue > s.max || targetValue < s.min) {
        stateRef->mult *= -1;
        targetValue = eval();

    } else if (stateRef->accum == s.maxFrames) {
        auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());
        bool flip = gen();

        if (flip) {
            stateRef->mult *= -1;
            targetValue = eval();
        }
    }
    stateRef->value = targetValue;
}

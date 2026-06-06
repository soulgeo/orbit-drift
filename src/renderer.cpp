#include "renderer.hpp"

#include <iostream>
#include <ostream>
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
    std::vector<CameraProfile> cameraProfiles;
};

struct RendererImpl {
    std::unordered_map<std::string, Model> models;
    std::unordered_map<std::string, std::tuple<Model, Matrix, Color>>
    objModelRefs;
};

CameraManager::CameraManager(Scene* c_scene) : scene(c_scene) {
    impl_ = new CameraManagerImpl;

    // Define Camera Profiles
    CameraProfile defaultCamera;
    defaultCamera.target = c_scene->getGameObject("player");
    defaultCamera.posLocalOffset = (Vector3) {0.0f, 1.0f, -2.5f};
    defaultCamera.targLocalOffset = (Vector3) {0.0f, 0.0f, 3.0f};
    defaultCamera.fovy = 60.0f;
    impl_->cameraProfiles.push_back(defaultCamera);

    CameraProfile inGravityCamera;
    inGravityCamera.target = c_scene->getGameObject("player");
    inGravityCamera.posLocalOffset = (Vector3) {0.0f, 1.0f, -3.8f};
    inGravityCamera.targLocalOffset = (Vector3) {0.0f, 0.0f, 3.0f};
    inGravityCamera.fovy = 80.0f;
    impl_->cameraProfiles.push_back(inGravityCamera);

    // Init camera
    activeProfile = CP_DEFAULT;
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

}

void CameraManager::update() {
    CameraProfile& active = impl_->cameraProfiles[activeProfile];

    // If new transition is starting, save the current camera state
    if (newProfile >= 0 && transIter == 0) {
        savedState.posOffset = currentState.posOffset;
        savedState.targOffset = currentState.targOffset;
        savedState.posLocalOffset = currentState.posLocalOffset;
        savedState.targLocalOffset = currentState.targLocalOffset;
        savedState.fovy = currentState.fovy;
    }

    if (newProfile < 0) {
        // Just set calced offsets to the active profile ones
        currentState.posOffset = active.posOffset;
        currentState.targOffset = active.targOffset;
        currentState.posLocalOffset = active.posLocalOffset;
        currentState.targLocalOffset = active.targLocalOffset;
        currentState.fovy = active.fovy;
    } else {
        // Get calced offsets for transition from saved state to new profile
        CameraProfile& next = impl_->cameraProfiles[newProfile];
        currentState.targOffset = Vector3Lerp(
            savedState.targOffset, 
            next.targOffset, 
            (float)transIter/maxTransIter
        );
        currentState.posOffset = Vector3Lerp(
            savedState.posOffset, 
            next.posOffset, 
            (float)transIter/maxTransIter
        );
        currentState.targLocalOffset = Vector3Lerp(
            savedState.targLocalOffset, 
            next.targLocalOffset, 
            (float)transIter/maxTransIter
        );
        currentState.posLocalOffset  = Vector3Lerp(
            savedState.posLocalOffset, 
            next.posLocalOffset, 
            (float)transIter/maxTransIter
        );
        currentState.fovy = Lerp(
            savedState.fovy, 
            next.fovy, 
            (float)transIter/maxTransIter
        );
    }

    // Update Camera Target
    GameObject& cameraTarget = *active.target;

    Vector3 targetPosition = cameraTarget.getPosition();
    Vector3 targetForward = cameraTarget.getForward();
    Vector3 targetUp = cameraTarget.getUp();
    Vector3 targetRight = cameraTarget.getRight();

    float followSpeed = 30.0f;

    Vector3 targConvertedOffset;
    // Translate local offset from local to upright space
    Vector3 rightOffset = currentState.targLocalOffset.x * targetRight;
    Vector3 upOffset = currentState.targLocalOffset.y * targetUp;
    Vector3 forwardOffset = currentState.targLocalOffset.z * targetForward;
    targConvertedOffset = rightOffset + upOffset + forwardOffset;

    // Finally translate to world space
    Vector3 worldTarget = targetPosition + currentState.targOffset + targConvertedOffset;

    camera.target = Vector3Lerp(camera.target, worldTarget, followSpeed*GetFrameTime());

    Vector3 posConvertedOffset;
    // Translate local offset from local to upright space
    rightOffset = currentState.posLocalOffset.x * targetRight;
    upOffset = currentState.posLocalOffset.y * targetUp;
    forwardOffset = currentState.posLocalOffset.z * targetForward;
    posConvertedOffset = rightOffset + upOffset + forwardOffset;

    // Finally translate to world space
    Vector3 worldPosition = targetPosition + currentState.posOffset + posConvertedOffset;

    camera.position = Vector3Lerp(camera.position, worldPosition, followSpeed*GetFrameTime());

    // Update Camera Up
    camera.up = targetUp;

    // Manage transition loop
    if (newProfile >= 0) {
        if (transIter >= maxTransIter) {
            activeProfile = newProfile;
            newProfile = -1;
            transIter = 0;
        } else {
            transIter++;
        }
    }
}

Renderer::Renderer(Scene *r_scene) : scene(r_scene), camManager(r_scene) {
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

    impl_->objModelRefs["player"] = {
        impl_->models["player_model"],
        MatrixRotateX(180.0f * DEG2RAD) * 
            MatrixRotateZ(180.0f * DEG2RAD) * 
            MatrixScale(0.2f, 0.2f, 0.2f),
        MAGENTA};

    float scalar;

    scalar = ((Planet *)r_scene->getGameObject("planet1"))->radius;
    impl_->objModelRefs["planet1"] = {impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar),
        PURPLE};

    scalar = ((Planet *)r_scene->getGameObject("planet2"))->radius;
    impl_->objModelRefs["planet2"] = {impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar), GREEN};

    scalar = ((Planet *)r_scene->getGameObject("planet3"))->radius;
    impl_->objModelRefs["planet3"] = {impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar),
        YELLOW};

    scalar = ((Planet *)r_scene->getGameObject("planet4"))->radius;
    impl_->objModelRefs["planet4"] = {impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar), BLUE};

    // Init shaders
    fog = LoadShader(TextFormat("resources/shaders/ambient.vert", GLSL_VERSION),
                     TextFormat("resources/shaders/ambient.frag", GLSL_VERSION));

    for (auto &[name, tuple] : impl_->objModelRefs) {
        std::get<0>(tuple).materials[0].shader = fog;
    }
}

Renderer::~Renderer() {
    for (auto &[name, model] : impl_->models) {
        UnloadModel(model);
    }
    UnloadShader(fog);
    delete impl_;
}


void Renderer::update() {
    dt = GetFrameTime();

    int camProfile = camManager.getProfile(); 
    auto player = (PlayerShip *)scene->getGameObject("player");
    if (player->enteredGravitySOI && camProfile == CP_DEFAULT) {
        camManager.switchProfile(CP_IN_GRAVITY);
    }
    if (player->exitedGravitySOI && camProfile == CP_IN_GRAVITY) {
        camManager.switchProfile(CP_DEFAULT);
    }

    camManager.update();

    // Update model transforms
    for (auto &[name, tuple] : impl_->objModelRefs) {
        GameObject *object = scene->getGameObject(name);
        if (object) {
            std::get<0>(tuple).transform =
                std::get<1>(tuple) * object->transform;
        }
    }
}

void Renderer::draw3D() {
    int distLoc = GetShaderLocation(fog, "viewPos");
    SetShaderValue(fog, distLoc, &camManager.camera.position, SHADER_UNIFORM_VEC3);
    SetShaderValue(fog, fog.locs[SHADER_LOC_VECTOR_VIEW], &camManager.camera.position,
                   SHADER_UNIFORM_VEC3);

    for (auto &[name, tuple] : impl_->objModelRefs) {
        Model &model = std::get<0>(tuple);
        DrawModel(model, Vector3Zero(), 1.0, std::get<2>(tuple));
        // DrawModelWires(model, Vector3Zero(), 1.0f,
        //                ColorTint(std::get<2>(tuple), (Color){200, 200, 200,
        //                255}));
    }
}

void Renderer::drawUI() {
    // auto text = "CAM PROFILE: " + std::to_string(camManager.getProfile());
    // DrawText(text.c_str(), 30, 50, 20, YELLOW);
    // text = "CAM TRANSITION PROFILE: " + std::to_string(camManager.getTransitionProfile());
    // DrawText(text.c_str(), 30, 75, 20, YELLOW);
    // text = "CAM TRANSITION ITERATOR: " + std::to_string(camManager.getTransIter());
    // DrawText(text.c_str(), 30, 100, 20, YELLOW);
    //
    // text = "CAM CURRENT TARG OFFSET X: " + std::to_string(camManager.currentState.targOffset.x);
    // DrawText(text.c_str(), 30, 150, 20, YELLOW);
    // text = "CAM CURRENT TARG OFFSET Y: " + std::to_string(camManager.currentState.targOffset.y);
    // DrawText(text.c_str(), 30, 175, 20, YELLOW);
    // text = "CAM CURRENT TARG OFFSET Z: " + std::to_string(camManager.currentState.targOffset.z);
    // DrawText(text.c_str(), 30, 200, 20, YELLOW);
    //
    // text = "CAM CURRENT POS OFFSET X: " + std::to_string(camManager.currentState.posOffset.x);
    // DrawText(text.c_str(), 30, 250, 20, YELLOW);
    // text = "CAM CURRENT POS OFFSET Y: " + std::to_string(camManager.currentState.posOffset.y);
    // DrawText(text.c_str(), 30, 275, 20, YELLOW);
    // text = "CAM CURRENT POS OFFSET Z: " + std::to_string(camManager.currentState.posOffset.z);
    // DrawText(text.c_str(), 30, 300, 20, YELLOW);
    //
    // text = "CAM CURRENT TARG LOCAL OFFSET X: " + std::to_string(camManager.currentState.targLocalOffset.x);
    // DrawText(text.c_str(), 30, 350, 20, YELLOW);
    // text = "CAM CURRENT TARG LOCAL OFFSET Y: " + std::to_string(camManager.currentState.targLocalOffset.y);
    // DrawText(text.c_str(), 30, 375, 20, YELLOW);
    // text = "CAM CURRENT TARG LOCAL OFFSET Z: " + std::to_string(camManager.currentState.targLocalOffset.z);
    // DrawText(text.c_str(), 30, 400, 20, YELLOW);
    //
    // text = "CAM CURRENT POS LOCAL OFFSET X: " + std::to_string(camManager.currentState.posLocalOffset.x);
    // DrawText(text.c_str(), 30, 450, 20, YELLOW);
    // text = "CAM CURRENT POS LOCAL OFFSET Y: " + std::to_string(camManager.currentState.posLocalOffset.y);
    // DrawText(text.c_str(), 30, 475, 20, YELLOW);
    // text = "CAM CURRENT POS LOCAL OFFSET Z: " + std::to_string(camManager.currentState.posLocalOffset.z);
    // DrawText(text.c_str(), 30, 500, 20, YELLOW);
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

void Renderer::_switchProfile(int profileId) {

}

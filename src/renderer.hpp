#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "scene.hpp"
#include "game_object.hpp"

struct CameraProfile {
    GameObject* target;
    Vector3 posOffset = (Vector3) {0.0f, 0.0f, 0.0f};
    Vector3 targOffset = (Vector3) {0.0f, 0.0f, 0.0f};
    Vector3 posLocalOffset = (Vector3) {0.0f, 0.0f, 0.0f};
    Vector3 targLocalOffset = (Vector3) {0.0f, 0.0f, 0.0f};
    float fovy;
};

struct CameraState {
    Vector3 posOffset;
    Vector3 targOffset;
    Vector3 posLocalOffset;
    Vector3 targLocalOffset;
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

    int getProfile() {
        return activeProfile;
    }
    
    int getTransitionProfile() {
        return newProfile;
    }

    int getTransIter() {
        return transIter;
    }

    void switchProfile(int targetProfile){
        newProfile = targetProfile;
        transIter = 0;
    };

    void update();

    CameraState currentState;
    CameraState savedState;

private:
    CameraManagerImpl* impl_;
    Scene* scene;
    int activeProfile = CP_DEFAULT;
    int newProfile = -1;
    int transIter = 0;
    int maxTransIter = 16;
};

struct RendererImpl;

class Renderer {
public:
    Renderer(Scene* scene);
    ~Renderer();

    CameraManager camManager;

    void update();
    void draw3D();
    void drawUI();

private:
    Scene* scene;
    Shader fog;
    RendererImpl* impl_;

    float dt;

    void _switchProfile(int profileId);
};

#endif // !RENDERER_HPP

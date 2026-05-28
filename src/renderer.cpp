#include "renderer.hpp"

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

struct RendererImpl {
    std::unordered_map<std::string, Model> models;
    std::unordered_map<std::string, std::tuple<Model, Matrix, Color>> objModelRefs;
};

Renderer::Renderer(Scene* r_scene) : scene(r_scene){
    impl_ = new RendererImpl;

    // Init camera
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    cameraTarget = r_scene->getGameObject("player");

    // Init models
    impl_->models["player_model"] = LoadModelFromMesh(GenMeshCone(0.2f, 0.5f, 16.0f));
    impl_->models["planet_model"] = LoadModelFromMesh(GenMeshSphere(1.0f, 30.0f, 30.0f));

    impl_->objModelRefs["player"] = {
        impl_->models["player_model"],
        MatrixRotateX(-90.0f * DEG2RAD),
        MAGENTA
    };

    float scalar;

    scalar = ((Planet*)r_scene->getGameObject("planet1"))->radius;
    impl_->objModelRefs["planet1"] = {
        impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar),
        PURPLE
    };

    scalar = ((Planet*)r_scene->getGameObject("planet2"))->radius;
    impl_->objModelRefs["planet2"] = {
        impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar),
        GREEN
    };

    scalar = ((Planet*)r_scene->getGameObject("planet3"))->radius;
    impl_->objModelRefs["planet3"] = {
        impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar),
        YELLOW
    };

    scalar = ((Planet*)r_scene->getGameObject("planet4"))->radius;
    impl_->objModelRefs["planet4"] = {
        impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar),
        BLUE
    };

    // Init shaders
    fog =
        LoadShader(TextFormat("resources/shaders/ambient.vert", GLSL_VERSION),
                   TextFormat("resources/shaders/ambient.frag", GLSL_VERSION));

    for (auto& [name, tuple] : impl_->objModelRefs) {
        std::get<0>(tuple).materials[0].shader = fog;
    }
}

Renderer::~Renderer() {
    for (auto& [name, model] : impl_->models) {
        UnloadModel(model);
    }
    UnloadShader(fog);
    delete impl_;
}

void Renderer::update() {
    // Update camera
    Vector3 position = cameraTarget->getPosition();
    Vector3 forward = cameraTarget->getForward();
    Vector3 up = cameraTarget->getUp();

    camera.target = position;
    Vector3 lookOffset = Vector3Scale(forward, -3.0f);
    Vector3 heightOffset = Vector3Scale(up, 2.5f);
    camera.position =
        Vector3Add(Vector3Add(position, lookOffset), heightOffset);
    camera.up = up;

    // Update model transforms
    for (auto& [name, tuple] : impl_->objModelRefs) {
        GameObject *object = scene->getGameObject(name);
        if (object) {
            std::get<0>(tuple).transform = MatrixMultiply(std::get<1>(tuple), object->transform);
        }
    }
}

void Renderer::draw3D() {
    int distLoc = GetShaderLocation(fog, "viewPos");
    SetShaderValue(fog, distLoc, &camera.position, SHADER_UNIFORM_VEC3);
    SetShaderValue(fog, fog.locs[SHADER_LOC_VECTOR_VIEW], &camera.position, SHADER_UNIFORM_VEC3);

    for (auto& [name, tuple] : impl_->objModelRefs) {
        Model& model = std::get<0>(tuple);
        DrawModel(model, Vector3Zero(), 1.0, std::get<2>(tuple));
        DrawModelWires(model, Vector3Zero(), 1.0f,
                       ColorTint(std::get<2>(tuple), (Color){200, 200, 200, 255}));
    }
}

void Renderer::drawUI() {
    if (((PlayerShip*)cameraTarget)->isInGravitySOI) {
        DrawText("IN GRAVITY", 20, 40, 32, YELLOW);
    }
}

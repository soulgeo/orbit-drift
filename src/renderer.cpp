#include "renderer.hpp"

#include <string>
#include <tuple>
#include <unordered_map>

#include "entities.hpp"
#include "game.hpp"
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
Renderer::Renderer(Game& game) {
    impl_ = new RendererImpl;
    impl_->models["player_model"] = LoadModelFromMesh(GenMeshCone(0.2f, 0.5f, 16.0f));
    impl_->models["planet_model"] = LoadModelFromMesh(GenMeshSphere(1.0f, 30.0f, 30.0f));

    impl_->objModelRefs["player"] = {
        impl_->models["player_model"],
        MatrixRotateX(-90.0f * DEG2RAD),
        MAGENTA
    };

    float scalar;

    scalar = ((Planet*)game.getGameObject("planet1"))->radius;
    impl_->objModelRefs["planet1"] = {
        impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar),
        PURPLE
    };

    scalar = ((Planet*)game.getGameObject("planet2"))->radius;
    impl_->objModelRefs["planet2"] = {
        impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar),
        GREEN
    };

    scalar = ((Planet*)game.getGameObject("planet3"))->radius;
    impl_->objModelRefs["planet3"] = {
        impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar),
        YELLOW
    };

    scalar = ((Planet*)game.getGameObject("planet4"))->radius;
    impl_->objModelRefs["planet4"] = {
        impl_->models["planet_model"],
        MatrixScale(scalar, scalar, scalar),
        BLUE
    };

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

void Renderer::update(Game& game) {
    for (auto& [name, tuple] : impl_->objModelRefs) {
        GameObject *object = game.getGameObject(name);
        if (object) {
            std::get<0>(tuple).transform = MatrixMultiply(std::get<1>(tuple), object->transform);
        }
    }
}

void Renderer::draw(Vector3 cameraPos, const Game& game) {
    int distLoc = GetShaderLocation(fog, "viewPos");
    SetShaderValue(fog, distLoc, &cameraPos, SHADER_UNIFORM_VEC3);
    SetShaderValue(fog, fog.locs[SHADER_LOC_VECTOR_VIEW], &cameraPos, SHADER_UNIFORM_VEC3);

    for (auto& [name, tuple] : impl_->objModelRefs) {
        Model& model = std::get<0>(tuple);
        DrawModel(model, Vector3Zero(), 1.0, std::get<2>(tuple));
        DrawModelWires(model, Vector3Zero(), 1.0f,
                       ColorTint(std::get<2>(tuple), (Color){200, 200, 200, 255}));
    }
}

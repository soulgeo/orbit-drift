#include "game_input.hpp"
#include "setup_game.hpp"
#include <raylib.h>
#include <raymath.h>

#include "game/control_component.hpp"
#include "game/g_boost_component.hpp"
#include "game/planet_factory.hpp"
#include "game/global_control_component.hpp"

namespace OrbitDrift {

void bind_inputs(Sputnik::Engine& engine) {
    using namespace Sputnik;
    auto& input = engine.input_handler();
    input.bind_key(KEY_SPACE, DOWN, INPUT_MOVE_UP);
    input.bind_key(KEY_LEFT_SHIFT, DOWN, INPUT_MOVE_DOWN);
    input.bind_key(KEY_W, DOWN, INPUT_MOVE_FORWARD);
    input.bind_key(KEY_S, DOWN, INPUT_MOVE_BACK);
    input.bind_key(KEY_A, DOWN, INPUT_MOVE_LEFT);
    input.bind_key(KEY_D, DOWN, INPUT_MOVE_RIGHT);
    input.bind_key(KEY_E, DOWN, INPUT_ROLL_CW);
    input.bind_key(KEY_Q, DOWN, INPUT_ROLL_CCW);
    input.bind_key(KEY_LEFT_ALT, PRESSED, INPUT_PAUSE);
    input.bind_key(KEY_F3, PRESSED, INPUT_DEBUG);
}


std::unique_ptr<Sputnik::Scene> create_main_scene(Sputnik::Engine* engine) {
    using namespace Sputnik;

    auto scene = std::make_unique<Scene>(engine);

    ResourceManager* rsrc_manager = &engine->resource_manager();
    Renderer* renderer = &engine->renderer();
    Physics* physics = &engine->physics();

    // Player setup
    auto player = std::make_unique<GameObject>(engine);
    auto player_model = rsrc_manager->load_model("resources/models/scene.gltf");
    auto texture = rsrc_manager->load_texture("resources/models/textures/Material_baseColor.png");
    player_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    player_model.transform *= MatrixRotateX(180.0f * DEG2RAD) 
        * MatrixRotateZ(180.0f * DEG2RAD) 
        * MatrixScale(0.2f, 0.2f, 0.2f);
    
    auto player_rend = std::make_unique<RenderableComponent>(player.get(), renderer, player_model);
    player_rend->set_initial_transform(player_model.transform);
    player_rend->set_color(RAYWHITE);
    player->add_component(std::move(player_rend));

    auto player_physics = std::make_unique<PhysicsComponent>(
        player.get(), physics, Vector3Zero(), Vector3Zero(), 10.0f);    
    player_physics->set_drag(10.0f);
    player->add_component(std::move(player_physics));
    
    player->add_component(std::make_unique<ColliderComponent>(
        player.get(), physics, Vector3Zero(), 2.0f, true));
    player->add_component(std::make_unique<ControlComponent>(player.get()));
    player->add_component(std::make_unique<DebugComponent>(player.get(), renderer));
    player->add_component(std::make_unique<GBoostComponent>(player.get()));

    scene->add_game_object("player", std::move(player));

    // Planets
    PlanetFactory planet_factory(engine);
    scene->add_game_object("planet1", planet_factory.create(
        (Vector3){-60.0f, 10.0f, -500.0f}, 40.0f, 200.0f, 7500.0f, PURPLE));
    scene->add_game_object("planet2", planet_factory.create(
        (Vector3){-300.0f, 20.0f, -1500.0f}, 80.0f, 320.0f, 1150.0f, GREEN));
    scene->add_game_object("planet3", planet_factory.create(
        (Vector3){600.0f, -60.0f, -2800.0f}, 160.0f, 480.0f, 15000.0f, YELLOW));
    scene->add_game_object("planet4", planet_factory.create(
        (Vector3){240.0f, -140.0f, -4500.0f}, 20.0f, 120.0f, 4500.0f, BLUE));

    // Camera setup
    auto camera_body = std::make_unique<GameObject>(engine);
    camera_body->add_component(
        std::make_unique<CameraComponent>(camera_body.get(), scene.get(), renderer)
    );
    camera_body->add_component(
        std::make_unique<DebugComponent>(camera_body.get(), renderer)
    );
    camera_body->add_component(
        std::make_unique<GlobalControlComponent>(camera_body.get())
    );
    
    scene->add_game_object("camera_body", std::move(camera_body));

    return scene;
}

}

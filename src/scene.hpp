#ifndef SCENE_HPP
#define SCENE_HPP

#include "game_object.hpp"
#include "input_handler.hpp"
#include <string>
#include <functional>

enum SceneInput {
    NONE = 0,
    PAUSE,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_FORWARD,
    MOVE_BACK,
    MOVE_LEFT,
    MOVE_RIGHT,
    ROLL_CW,
    ROLL_CCW,
};

struct SceneImpl;

class Scene {
public:
    Scene();
    ~Scene();

    static constexpr size_t MAX_GAMEOBJECT_COUNT = 200;

    GameObject* get_game_object(const std::string& name);
    void for_each_game_object(std::function<void(const std::string&, GameObject&)> func);

    InputHandler input_handler;

    bool is_paused = false;
    bool is_active_input(int input);

    void update();

private:
    SceneImpl* impl_;
};

#endif // SCENE_HPP

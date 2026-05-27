#ifndef GAME_HPP
#define GAME_HPP

#include "game_object.hpp"
#include "input_handler.hpp"
#include <string>
#include <functional>

enum GameInput {
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

struct GameImpl;

class Game {
public:
    Game();
    ~Game();

    static constexpr size_t MAX_GAMEOBJECT_COUNT = 200;

    GameObject* getGameObject(const std::string& name);
    void forEachGameObject(std::function<void(const std::string&, GameObject&)> func);

    InputHandler inputHandler;

    bool isPaused = false;

    bool isActiveInput(int input);

    void update();

private:
    GameImpl* impl_;
};

#endif // GAME_HPP

#ifndef GAME_HPP
#define GAME_HPP

#include "game_object.hpp"
#include "input_handler.hpp"
#include <string>
#include <functional>

struct GameImpl;

class Game {
public:
    Game();
    ~Game();

    static constexpr size_t MAX_GAMEOBJECT_COUNT = 200;

    GameObject* getGameObject(std::string name);
    void forEachGameObject(std::function<void(GameObject&)> func);

    InputHandler inputHandler;

    bool isPaused = false;

    bool isActiveInput(std::string input);

    void update();

    void draw();

private:

    Shader fog;
    GameImpl* impl_;
};

#endif // GAME_HPP

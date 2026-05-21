#ifndef GAME_HPP
#define GAME_HPP

#include "game_object.hpp"
#include "input_handler.hpp"
#include <string>

struct GameImpl;

class Game {
public:
    Game();
    ~Game();

    static constexpr size_t MAX_GAMEOBJECT_COUNT = 200;

    GameObject* getGameObject(std::string name);

    InputHandler inputHandler;

    void update();
private:
    GameImpl* impl_;
};

#endif // GAME_HPP

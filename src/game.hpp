#ifndef GAME_HPP
#define GAME_HPP

#include "game_object.hpp"
#include <string>

struct GameImpl;

class Game {
public:
    Game();
    ~Game();

    static constexpr size_t MAX_GAMEOBJECT_COUNT = 200;

    GameObject* getGameObject(std::string name);

private:
    GameImpl* impl_;
};

#endif // GAME_HPP

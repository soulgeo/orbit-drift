#include "game_object.hpp"

class PlayerShip : public GameObject {
    float forwardSpeed = 0.2f;
    float panSpeed = 0.004f;
    float rollSpeed = 0.02f;

    void update() override;
};

class Planet : public GameObject {

};

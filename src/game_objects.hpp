#include "game_object.hpp"

class PlayerShip : public GameObject {
public:
    float forwardSpeed = 12.0f;
    float panSpeed = 0.004f;
    float rollSpeed = 1.0f;

    void update() override;
    void handleAction(const std::string& action) override;
};

class Planet : public GameObject {

};

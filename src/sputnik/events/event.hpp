#pragma once

#include "ecs/game_object.hpp"
#include <string>

namespace Sputnik {

struct Event {
    std::string message;
    GameObject* sender;
};

}

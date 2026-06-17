#pragma once

#include <raylib.h>
#include <vector>
#include "sputnik/ecs/component.hpp"
#include "sputnik/ecs/game_object.hpp"

namespace Sputnik {

struct SoundThing {
    const char* name;
    Sound sound;
    float volume;
    float pitch;
    bool loop;
};

class AudioComponent : public Component {
    std::vector<Sound> sounds_;

public:
    AudioComponent(GameObject* owner);
    ~AudioComponent();

    void add_sound(Sound sound);
};

}

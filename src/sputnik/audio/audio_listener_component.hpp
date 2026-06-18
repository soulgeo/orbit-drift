#pragma once

#include "ecs/component.hpp"
#include "ecs/game_object.hpp"
#include "ecs/transform_component.hpp"
namespace Sputnik {

class AudioListenerComponent : public Component {
    TransformComponent* transform_;
public:
    AudioListenerComponent(GameObject* owner);
    ~AudioListenerComponent();

    void init() override;

    Vector3 position();
    Vector3 forward();
    Vector3 up();
};

}

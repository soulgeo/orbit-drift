#pragma once

#include "ecs/component.hpp"
#include "ecs/game_object.hpp"
#include "events/event_dispatcher.hpp"

namespace Sputnik {

class EventComponent : public Component {
    EventDispatcher* event_dsp_;
public:
    EventComponent(GameObject* owner, EventDispatcher* event_dsp);
    ~EventComponent();

    void send(std::string_view message);
};

}

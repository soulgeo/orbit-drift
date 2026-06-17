#pragma once

#include "ecs/component.hpp"
#include "events/event.hpp"
#include <vector>

namespace Sputnik {

class EventDispatcher {
    std::vector<Event> events_;
    std::vector<Component*> subscribers_;

public:
    EventDispatcher();
    ~EventDispatcher();

    void add_event(Event event);
    void add_subscriber(Component* component);
    void update();
};

}

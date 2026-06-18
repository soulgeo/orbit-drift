#include "events/event_component.hpp"
#include "ecs/game_object.hpp"
#include "events/event_dispatcher.hpp"

namespace Sputnik {

EventComponent::EventComponent(GameObject* owner, EventDispatcher* event_dsp) : 
    Component(owner), event_dsp_(event_dsp) {}

EventComponent::~EventComponent() = default;

void EventComponent::send(std::string_view message) {
    Event event = {std::string(message), owner_};
    event_dsp_->add_event(event);
}

}

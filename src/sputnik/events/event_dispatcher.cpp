#include "events/event_dispatcher.hpp"

namespace Sputnik {

EventDispatcher::EventDispatcher() {}

EventDispatcher::~EventDispatcher() {}

void EventDispatcher::add_event(Event event) {
    events_.push_back(event);
}

void EventDispatcher::update() {
    for (auto i = events_.begin(); i != events_.end(); ++i){
        for (auto j = subscribers_.begin(); j != subscribers_.end(); ++j) {
            (*j)->on_event(*i);
        }
    }
    events_.clear();
}

}

#include "sputnik/ecs/component.hpp"
#include "sputnik/ecs/game_object.hpp"

namespace Sputnik {

    Component::Component(GameObject* owner) : owner_(owner) {}

    Component::~Component() {}

    void Component::start() {}
    void Component::early_update() {}
    void Component::fixed_update() {}
    void Component::update() {}
    void Component::late_update() {}


    void Component::on_collision_enter(GameObject* other) {}
    void Component::on_collision_exit(GameObject* other) {}
    void Component::on_trigger_enter(GameObject* other) {}
    void Component::on_trigger_stay(GameObject* other) {}
    void Component::on_trigger_exit(GameObject* other) {}

}

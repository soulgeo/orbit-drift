#include "engine.hpp"
#include "transform_component.hpp"
#include "game_object.hpp"
#include <memory>
#include <vector>

GameObject::GameObject(Engine* engine) {
    engine_ = engine;

    auto transform_comp = std::make_unique<TransformComponent>(this, engine_);
    transform_ = transform_comp.get();
    add_component(std::move(transform_comp));
}

GameObject::~GameObject() = default;

void GameObject::add_component(std::unique_ptr<Component> comp) {
    components_.push_back(std::move(comp));
}

size_t GameObject::get_component_count() const {
    return components_.size();
}

Component* GameObject::get_component_at(size_t index) const {
    return components_[index].get();
}

TransformComponent& GameObject::transform() {
    return *transform_;
}

Engine* GameObject::get_engine() {
    return engine_;
}

void GameObject::start() {
    transform_ = get_component<TransformComponent>();
    for (auto i = components_.begin(); i != components_.end(); i++) {
        (*i)->start();
    }
}

void GameObject::update() {
    for (auto i = components_.begin(); i != components_.end(); i++) {
        (*i)->update();
    }
}

void GameObject::early_update() {
    for (auto i = components_.begin(); i != components_.end(); i++) {
        (*i)->early_update();
    }
}

void GameObject::late_update() {
    for (auto i = components_.begin(); i != components_.end(); i++) {
        (*i)->late_update();
    }
}

void GameObject::fixed_update() {
    for (auto i = components_.begin(); i != components_.end(); i++) {
        (*i)->fixed_update();
    }
}

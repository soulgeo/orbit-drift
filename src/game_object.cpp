#include "engine.hpp"
#include "transform_component.hpp"
#include "game_object.hpp"
#include <memory>
#include <vector>

struct GameObject::Impl {
    std::vector<std::unique_ptr<Component>> components;
    TransformComponent* transform;
    Engine* engine;
};

GameObject::GameObject() {
    impl_ = std::make_unique<Impl>();

    auto transform_comp = std::make_unique<TransformComponent>(this, impl_->engine);
    add_component(std::move(transform_comp));
}

GameObject::~GameObject() = default;

void GameObject::add_component(std::unique_ptr<Component> comp) {
    impl_->components.push_back(std::move(comp));
}

size_t GameObject::get_component_count() const {
    return impl_->components.size();
}

Component* GameObject::get_component_at(size_t index) const {
    return impl_->components[index].get();
}

TransformComponent& GameObject::transform() {
    return *impl_->transform;
}

Engine* GameObject::get_engine() {
    return impl_->engine;
}

void GameObject::start() {
    for (auto i = impl_->components.begin(); i != impl_->components.end(); i++) {
        (*i)->start();
    }
    impl_->transform = get_component<TransformComponent>();
}

void GameObject::update() {
    for (auto i = impl_->components.begin(); i != impl_->components.end(); i++) {
        (*i)->update();
    }
}

void GameObject::early_update() {
    for (auto i = impl_->components.begin(); i != impl_->components.end(); i++) {
        (*i)->early_update();
    }
}

void GameObject::late_update() {
    for (auto i = impl_->components.begin(); i != impl_->components.end(); i++) {
        (*i)->late_update();
    }
}

void GameObject::fixed_update() {
    for (auto i = impl_->components.begin(); i != impl_->components.end(); i++) {
        (*i)->fixed_update();
    }
}

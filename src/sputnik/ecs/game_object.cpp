#include "sputnik/core/engine.hpp"
#include "sputnik/ecs/transform_component.hpp"
#include "sputnik/ecs/game_object.hpp"
#include <memory>
#include <vector>

namespace Sputnik {

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

    size_t GameObject::component_count() const {
        return components_.size();
    }

    Component* GameObject::component_at(size_t index) const {
        if (index < components_.size()) {
            return components_[index].get();
        }
        return nullptr;
    }

    TransformComponent& GameObject::transform() {
        return *transform_;
    }

    Engine* GameObject::engine() {
        return engine_;
    }

    void GameObject::init() {
        transform_ = component<TransformComponent>();
        for (auto i = components_.begin(); i != components_.end(); i++) {
            (*i)->init();
        }
    }

    void GameObject::early_update() {
        for (auto i = components_.begin(); i != components_.end(); i++) {
            (*i)->early_update();
        }
    }

    void GameObject::fixed_update() {
        for (auto i = components_.begin(); i != components_.end(); i++) {
            (*i)->fixed_update();
        }
    }

    void GameObject::update() {
        for (auto i = components_.begin(); i != components_.end(); i++) {
            (*i)->update();
        }
    }

    void GameObject::late_update() {
        for (auto i = components_.begin(); i != components_.end(); i++) {
            (*i)->late_update();
        }
    }

    void GameObject::on_collision_enter(GameObject* other) {
        for (auto i = components_.begin(); i != components_.end(); i++) {
            (*i)->on_collision_enter(other);
        }
    }

    void GameObject::on_collision_exit(GameObject* other) {
        for (auto i = components_.begin(); i != components_.end(); i++) {
            (*i)->on_collision_exit(other);
        }
    }

    void GameObject::on_trigger_enter(GameObject* other) {
        for (auto i = components_.begin(); i != components_.end(); i++) {
            (*i)->on_trigger_enter(other);
        }
    }

    void GameObject::on_trigger_stay(GameObject* other) {
        for (auto i = components_.begin(); i != components_.end(); i++) {
            (*i)->on_trigger_stay(other);
        }
    }

    void GameObject::on_trigger_exit(GameObject* other) {
        for (auto i = components_.begin(); i != components_.end(); i++) {
            (*i)->on_trigger_exit(other);
        }
    }

}

#pragma once

#include "sputnik/ecs/component.hpp"
#include <vector>
#include <memory>
#include <cstddef>

namespace Sputnik {

    class Engine;
    class TransformComponent;

    class GameObject {
        std::vector<std::unique_ptr<Component>> components_;
        Engine* engine_;

    public:
        GameObject(Engine* engine);
        ~GameObject();

        template<typename T, typename... Args>
        T* add_component(Args&&... args) {
            auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
            T* ptr = component.get();
            components_.push_back(std::move(component));
            return ptr; // Return raw pointer if further config is needed
        }

        size_t component_count() const;
        Component* component_at(size_t index) const;

        template <typename T> 
        T* component() {
            for (size_t i = 0; i < component_count(); ++i) {
                T* result = dynamic_cast<T*>(component_at(i));
                if (result) return result;
            }
            return nullptr;
        }

        void init();
        void early_update();
        void fixed_update();
        void update();
        void late_update();

        void on_collision_enter(GameObject* other);
        void on_collision_exit(GameObject* other);

        void on_trigger_enter(GameObject* other);
        void on_trigger_stay(GameObject* other);
        void on_trigger_exit(GameObject* other);

        Engine* engine();
    };

}

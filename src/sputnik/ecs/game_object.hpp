#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "sputnik/ecs/component.hpp"
#include <vector>
#include <memory>
#include <cstddef>

namespace Sputnik {

    class Engine;
    class TransformComponent;

    class GameObject {
        std::vector<std::unique_ptr<Component>> components_;
        TransformComponent* transform_;
        Engine* engine_;

    public:
        GameObject(Engine* engine);
        ~GameObject();

        void add_component(std::unique_ptr<Component> comp);

        size_t get_component_count() const;
        Component* get_component_at(size_t index) const;

        template <typename T> 
        T* get_component() {
            for (size_t i = 0; i < get_component_count(); ++i) {
                T* result = dynamic_cast<T*>(get_component_at(i));
                if (result) return result;
            }
            return nullptr;
        }

        void start();
        void early_update();
        void fixed_update();
        void update();
        void late_update();

        void on_collision_enter(GameObject* other);
        void on_collision_exit(GameObject* other);

        void on_trigger_enter(GameObject* other);
        void on_trigger_stay(GameObject* other);
        void on_trigger_exit(GameObject* other);

        TransformComponent& transform();

        Engine* get_engine();
    };

}

#endif // GAMEOBJECT_HPP

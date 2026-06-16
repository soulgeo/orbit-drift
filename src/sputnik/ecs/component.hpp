#ifndef COMPONENT_HPP
#define COMPONENT_HPP

namespace Sputnik {

    class GameObject;

    class Component {
    protected:
        GameObject* owner_;

    public:
        Component(GameObject* owner);
        virtual ~Component();

        virtual void start();
        virtual void early_update();
        virtual void fixed_update();
        virtual void update();
        virtual void late_update();

        virtual void on_collision_enter(GameObject* other);
        virtual void on_collision_exit(GameObject* other);

        virtual void on_trigger_enter(GameObject* other);
        virtual void on_trigger_stay(GameObject* other);
        virtual void on_trigger_exit(GameObject* other);
    };

}

#endif // COMPONENT_HPP

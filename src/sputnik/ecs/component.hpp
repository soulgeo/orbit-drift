#pragma once

namespace Sputnik {

    class GameObject;
    class Event;

    class Component {
    protected:
        GameObject* owner_;

    public:
        Component(GameObject* owner);
        virtual ~Component();

        virtual void init();
        virtual void early_update();
        virtual void fixed_update();
        virtual void update();
        virtual void late_update();

        virtual void on_collision_enter(GameObject* other);
        virtual void on_collision_exit(GameObject* other);

        virtual void on_trigger_enter(GameObject* other);
        virtual void on_trigger_stay(GameObject* other);
        virtual void on_trigger_exit(GameObject* other);

        virtual void on_event(Event event);
    };

}

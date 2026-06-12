#ifndef COMPONENT_HPP
#define COMPONENT_HPP

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

};

#endif // COMPONENT_HPP

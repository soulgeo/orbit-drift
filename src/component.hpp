#ifndef COMPONENT_HPP
#define COMPONENT_HPP

class GameObject;

class Component {
public:
    Component(GameObject* owner);
    virtual ~Component();
    virtual void update();

protected:
    GameObject* owner_;
};

#endif // COMPONENT_HPP

#include "component.hpp"

Component::Component(GameObject* owner) : owner_(owner) {}

Component::~Component() {}

void Component::update() {}


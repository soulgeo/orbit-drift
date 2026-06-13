#include "component.hpp"

Component::Component(GameObject* owner) : owner_(owner) {}

Component::~Component() {}

void Component::start() {}
void Component::early_update() {}
void Component::fixed_update() {}
void Component::update() {}
void Component::late_update() {}


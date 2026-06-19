#include "audio/audio_listener_component.hpp"
#include "ecs/component.hpp"
#include "ecs/transform_component.hpp"

namespace Sputnik {

AudioListenerComponent::AudioListenerComponent(GameObject* owner) : Component(owner) {}

AudioListenerComponent::~AudioListenerComponent() = default;

void AudioListenerComponent::init() {
    transform_ = owner_->component<TransformComponent>();
}

Vector3 AudioListenerComponent::position() {
    return transform_->visual_position();
}

Vector3 AudioListenerComponent::forward() {
    return transform_->visual_forward();
}

Vector3 AudioListenerComponent::up() {
    return transform_->visual_up();
}

}

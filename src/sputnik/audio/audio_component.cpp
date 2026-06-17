#include "sputnik/audio/audio_component.hpp"

namespace Sputnik {

AudioComponent::AudioComponent(GameObject* owner) : Component(owner) {}

AudioComponent::~AudioComponent() = default;

}

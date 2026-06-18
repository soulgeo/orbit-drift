#include "sputnik/audio/audio_source_component.hpp"
#include "audio/audio_listener_component.hpp"
#include "ecs/transform_component.hpp"
#include "events/event.hpp"
#include "raylib.h"
#include "raymath.h"
#include <string_view>

namespace Sputnik {

AudioSourceComponent::AudioSourceComponent(
    GameObject* owner, AudioListenerComponent* listener, EventDispatcher* event_dsp) : 
    Component(owner), listener_(listener) 
{
    event_dsp->add_subscriber(this);
}

AudioSourceComponent::~AudioSourceComponent() = default;

void AudioSourceComponent::init() {
    transform_ = owner_->component<TransformComponent>();
}

void AudioSourceComponent::update() {
    for (auto i = sounds_.begin(); i != sounds_.end(); ++i){
        Sound& sound = (*i).sound;
        float volume = (*i).volume;
        float pan = (*i).pan;
        float pitch = (*i).pitch;

        if (is_spatial_) {
            update_spatial();
            volume *= attenuation_;
            pan = Clamp(pan + spatial_pan_ - 0.5f, 0.0f, 1.0f);
        }

        SetSoundPan(sound, pan);
        SetSoundPitch(sound, pitch);
        SetSoundVolume(sound, volume);
    }
}

void AudioSourceComponent::update_spatial() {
    Vector3 direction = transform_->position() - listener_->position();
    float distance = Vector3Length(direction);

    attenuation_ = 1.0f/(1.0f + (distance/max_distance_));
    attenuation_ = Clamp(attenuation_, 0.0f, 1.0f);

    Vector3 normalizedDirection = Vector3Normalize(direction);
    Vector3 forward = Vector3Normalize(listener_->forward() - listener_->position());
    Vector3 right = Vector3Normalize(Vector3CrossProduct(listener_->up(), forward));

    float dotProduct = Vector3DotProduct(forward, normalizedDirection);
    if (dotProduct < 0.0f) attenuation_ *= (1.0f + dotProduct*0.5f);

    spatial_pan_ = 0.5f + 0.5f*Vector3DotProduct(normalizedDirection, right);
}

RichSound* AudioSourceComponent::rich_sound(std::string_view name) {
    auto it = sound_name_index_.find(std::string(name));
    if (it == sound_name_index_.end()) {
        return nullptr;
    }
    return &sounds_[it->second];
}

void AudioSourceComponent::add_sound(std::string_view name, Sound sound) {
    if (!IsSoundValid(sound)) return;
    RichSound rich_sound = {std::string(name), sound, 1.0f, 1.0f, 0.5f};
    sounds_.push_back(rich_sound);
    sound_name_index_[std::string(name)] = sounds_.size() - 1;
}

void AudioSourceComponent::set_volume(std::string_view name, float volume) {
    RichSound* r_sound = rich_sound(name);
    if (!r_sound) return;
    r_sound->volume = volume;
}

void AudioSourceComponent::set_pitch(std::string_view name, float pitch) {
    RichSound* r_sound = rich_sound(name);
    if (!r_sound) return;
    r_sound->pitch = pitch;
}

void AudioSourceComponent::set_pan(std::string_view name, float pan) {
    RichSound* r_sound = rich_sound(name);
    if (!r_sound) return;
    r_sound->pan = pan;
}

void AudioSourceComponent::play(std::string_view name) {    
    RichSound* r_sound = rich_sound(name);
    if (!r_sound) return;
    PlaySound(r_sound->sound);
}

void AudioSourceComponent::pause(std::string_view name) {
    RichSound* r_sound = rich_sound(name);
    if (!r_sound) return;
    PauseSound(r_sound->sound);
}

void AudioSourceComponent::resume(std::string_view name) {
    RichSound* r_sound = rich_sound(name);
    if (!r_sound) return;
    ResumeSound(r_sound->sound);
}

void AudioSourceComponent::stop(std::string_view name) {
    RichSound* r_sound = rich_sound(name);
    if (!r_sound) return;
    StopSound(r_sound->sound);
}

void AudioSourceComponent::assign_sound_to_event(
    std::string_view event_name, std::string_view sound_name, int action
) {
    event_to_sound_action_map_[std::string(event_name)] = 
        (SoundAction){std::string(sound_name), action};
}

void AudioSourceComponent::on_event(Event event){
    auto it = event_to_sound_action_map_.find(event.message);
    if (it == event_to_sound_action_map_.end()) return;

    RichSound* r_sound = rich_sound(it->second.name);
    if (!r_sound) return;

    switch (it->second.action) {
        case SOUND_ACTION_PLAY:
            PlaySound(r_sound->sound);
            break;
        case SOUND_ACTION_PAUSE:
            PauseSound(r_sound->sound);
            break;
        case SOUND_ACTION_RESUME:
            ResumeSound(r_sound->sound);
            break;
        case SOUND_ACTION_STOP:
            StopSound(r_sound->sound);
            break;
    }
}

}

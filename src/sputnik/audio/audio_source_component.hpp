#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>
#include "audio/audio_listener_component.hpp"
#include "ecs/transform_component.hpp"
#include "events/event_dispatcher.hpp"
#include "sputnik/ecs/component.hpp"
#include "sputnik/ecs/game_object.hpp"

namespace Sputnik {

struct RichSound {
    std::string name;
    Sound sound;
    float volume;
    float pitch;
    float pan;
};

enum {
    SOUND_ACTION_PLAY = 0,
    SOUND_ACTION_PAUSE,
    SOUND_ACTION_RESUME,
    SOUND_ACTION_STOP,
};

class AudioSourceComponent : public Component {
    std::vector<RichSound> sounds_;
    std::unordered_map<std::string, size_t> sound_name_index_;
    struct SoundAction {
        std::string name;
        int action;
    };
    std::unordered_map<std::string, SoundAction> event_to_sound_action_map_;

    bool is_spatial_;

    float attenuation_;
    float spatial_pan_;
    float max_distance_;
    TransformComponent* transform_;

    AudioListenerComponent* listener_;

public:
    AudioSourceComponent(
        GameObject* owner, AudioListenerComponent* listener, EventDispatcher* event_dsp);
    ~AudioSourceComponent();

    void init() override;
    void update() override;

    void on_event(Event event) override;

    RichSound* rich_sound(std::string_view name);
    void add_sound(std::string_view name, Sound sound);

    void set_volume(std::string_view name, float volume);
    void set_pitch(std::string_view name, float pitch);
    void set_pan(std::string_view name, float pan);

    void set_is_spatial(bool is_spatial);

    void update_spatial();

    void play(std::string_view name);
    void pause(std::string_view name);
    void resume(std::string_view name);
    void stop(std::string_view name);

    void assign_sound_to_event(std::string_view event_name, std::string_view sound_name, int action);
};

}

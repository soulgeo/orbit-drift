#pragma once

#include <string>
#include <unordered_map>
namespace Sputnik {

class AudioMixer {
public:
    struct Track {
        float volume;
        float pitch;
        float pan;
    };

    void add_mixer_track(Track track);

    void set_mixer_track_volume(const char* name, float volume);
    void set_mixer_track_pitch(const char* name, float volume);
    void set_mixer_track_pan(const char* name, float volume);

private:
    std::unordered_map<std::string, Track> mixer_tracks_;
};

}

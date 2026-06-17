#include "audio/audio_manager.hpp"
#include "raylib.h"

namespace Sputnik {

AudioManager::AudioManager() {
    InitAudioDevice();
}

AudioManager::~AudioManager() {
    CloseAudioDevice();
}

}

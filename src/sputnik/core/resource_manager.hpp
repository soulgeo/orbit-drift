#pragma once

#include <raylib.h>
#include <unordered_map>

namespace Sputnik {

    class ResourceManager {
        std::unordered_map<const char*, Model> models_;
        std::unordered_map<const char*, Texture2D> textures_;
        std::unordered_map<const char*, Sound> sounds_;

        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;

    public:
        ResourceManager();
        ~ResourceManager();

        Model load_model(const char* path);
        Texture2D load_texture(const char* path);
        Sound load_sound(const char* path);

        void unload_all_resources();
    };

}

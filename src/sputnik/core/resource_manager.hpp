#pragma once

#include <raylib.h>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Sputnik {

    class ResourceManager {
        std::unordered_map<std::string, Model> models_;
        std::unordered_map<std::string, Texture2D> textures_;
        std::unordered_map<std::string, Sound> sounds_;
        std::unordered_map<std::string, Shader> shaders_;

        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;

    public:
        ResourceManager();
        ~ResourceManager();

        Model load_model(std::string_view path);
        Texture2D load_texture(std::string_view path);
        Sound load_sound(std::string_view path);
        Shader load_shader(std::string_view vs_path, std::string_view fs_path);

        void unload_all_resources();
    };

}

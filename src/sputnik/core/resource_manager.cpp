#include "sputnik/core/resource_manager.hpp"
#include <raylib.h>
#include <iostream>
#include <string_view>
#include <unordered_map>

namespace Sputnik {

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
    unload_all_resources();
}

Model ResourceManager::load_model(std::string_view path) {
    auto path_string = std::string(path);
    if (models_.count(path_string)) {
        return models_[path_string];
    }

    Model model = ::LoadModel(path_string.c_str());
    if (model.meshes == nullptr) {
        std::cerr << "ERROR: Failed to load model: " << path_string << std::endl;
        return Model{};
    }

    models_[path_string] = model;
    return models_[path_string];
}

Texture2D ResourceManager::load_texture(std::string_view path) {
    auto path_string = std::string(path);
    if (textures_.count(path_string)) {
        return textures_[path_string];
    }

    Texture2D texture = ::LoadTexture(path_string.c_str());
    if (texture.id == 0) {
        std::cerr << "ERROR: Failed to load texture: " << path_string << std::endl;
        return Texture2D{};
    }

    textures_[path_string] = texture;
    return textures_[path_string];
}

Sound ResourceManager::load_sound(std::string_view path) {
    auto path_string = std::string(path);
    if (sounds_.count(path_string)) {
        return sounds_[path_string];
    }

    Sound sound = ::LoadSound(path_string.c_str());
    if (sound.frameCount == 0) {
        std::cerr << "ERROR: Failed to load sound: " << path << std::endl;
        return Sound{};
    }

    sounds_[path_string] = sound;
    return sounds_[path_string];
}

Shader ResourceManager::load_shader(std::string_view vs_path, std::string_view fs_path)
{
    auto vs_path_string = std::string(vs_path);
    auto fs_path_string = std::string(fs_path);
    auto path_string = vs_path_string + fs_path_string;
    if (shaders_.count(path_string)) {
        return shaders_[path_string];
    }

    Shader shader = ::LoadShader(vs_path_string.c_str(), fs_path_string.c_str());
    if (!IsShaderValid(shader)) {
        std::cerr << "ERROR: Failed to load shader: " << vs_path_string << ", " 
            << fs_path_string << std::endl;
        return Shader{};
    }

    shaders_[path_string] = shader;
    return shaders_[path_string];
}

void ResourceManager::unload_all_resources() {
    for (auto& pair : models_) {
        ::UnloadModel(pair.second);
    }
    models_.clear();

    for (auto& pair : textures_) {
        ::UnloadTexture(pair.second);
    }
    textures_.clear();

    for (auto& pair : sounds_) {
        ::UnloadSound(pair.second);
    }
    sounds_.clear();

    for (auto& pair : shaders_) {
        ::UnloadShader(pair.second);
    }

    std::cout << "INFO: Unloaded all resources." << std::endl;
}

}

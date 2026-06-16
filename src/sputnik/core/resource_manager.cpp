#include "sputnik/core/resource_manager.hpp"
#include <raylib.h>
#include <iostream>
#include <unordered_map>

namespace Sputnik {

    ResourceManager::ResourceManager() {
    }

    ResourceManager::~ResourceManager() {
        unload_all_resources();
    }

    Model ResourceManager::load_model(const char* path) {
        if (models_.count(path)) {
            return models_[path];
        }

        Model model = ::LoadModel(path);
        if (model.meshes == nullptr) {
            std::cerr << "ERROR: Failed to load model: " << path << std::endl;
            return Model{};
        }

        models_[path] = model;
        return models_[path];
    }

    Texture2D ResourceManager::load_texture(const char* path) {
        if (textures_.count(path)) {
            return textures_[path];
        }

        Texture2D texture = ::LoadTexture(path);
        if (texture.id == 0) {
            std::cerr << "ERROR: Failed to load texture: " << path << std::endl;
            return Texture2D{};
        }

        textures_[path] = texture;
        return textures_[path];
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
        std::cout << "INFO: Unloaded all resources." << std::endl;
    }

}

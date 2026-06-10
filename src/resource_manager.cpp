#include "resource_manager.hpp"
#include "raylib.h"
#include <iostream>
#include <memory>
#include <unordered_map>

struct ResourceManagerImpl {
    std::unordered_map<std::string, Model> models_;
    std::unordered_map<std::string, Texture2D> textures_;
};

ResourceManager::ResourceManager() {
    impl_ = std::make_unique<ResourceManagerImpl>();
}

ResourceManager::~ResourceManager() {
    UnloadAllResources();
}

Model ResourceManager::LoadModel(const std::string& path) {
    if (impl_->models_.count(path)) {
        return impl_->models_[path];
    }

    Model model = ::LoadModel(path.c_str());
    if (model.meshes == nullptr) {
        std::cerr << "ERROR: Failed to load model: " << path << std::endl;
        return Model{};
    }

    impl_->models_[path] = model;
    return impl_->models_[path];
}

Texture2D ResourceManager::LoadTexture(const std::string& path) {
    if (impl_->textures_.count(path)) {
        return impl_->textures_[path];
    }

    Texture2D texture = ::LoadTexture(path.c_str());
    if (texture.id == 0) {
        std::cerr << "ERROR: Failed to load texture: " << path << std::endl;
        return Texture2D{};
    }

    impl_->textures_[path] = texture;
    return impl_->textures_[path];
}

void ResourceManager::UnloadAllResources() {
    for (auto& pair : impl_->models_) {
        ::UnloadModel(pair.second);
    }
    impl_->models_.clear();

    for (auto& pair : impl_->textures_) {
        ::UnloadTexture(pair.second);
    }
    impl_->textures_.clear();
    std::cout << "INFO: Unloaded all resources." << std::endl;
}

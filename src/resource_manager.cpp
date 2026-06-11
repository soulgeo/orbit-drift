#include "resource_manager.hpp"
#include "raylib.h"
#include <iostream>
#include <memory>
#include <unordered_map>

struct ResourceManager::Impl {
    std::unordered_map<std::string, Model> models;
    std::unordered_map<std::string, Texture2D> textures;
};

ResourceManager::ResourceManager() {
    impl_ = std::make_unique<Impl>();
}

ResourceManager::~ResourceManager() {
    unload_all_resources();
}

Model ResourceManager::load_model(const char* path) {
    if (impl_->models.count(path)) {
        return impl_->models[path];
    }

    Model model = ::LoadModel(path);
    if (model.meshes == nullptr) {
        std::cerr << "ERROR: Failed to load model: " << path << std::endl;
        return Model{};
    }

    impl_->models[path] = model;
    return impl_->models[path];
}

Texture2D ResourceManager::load_texture(const char* path) {
    if (impl_->textures.count(path)) {
        return impl_->textures[path];
    }

    Texture2D texture = ::LoadTexture(path);
    if (texture.id == 0) {
        std::cerr << "ERROR: Failed to load texture: " << path << std::endl;
        return Texture2D{};
    }

    impl_->textures[path] = texture;
    return impl_->textures[path];
}

void ResourceManager::unload_all_resources() {
    for (auto& pair : impl_->models) {
        ::UnloadModel(pair.second);
    }
    impl_->models.clear();

    for (auto& pair : impl_->textures) {
        ::UnloadTexture(pair.second);
    }
    impl_->textures.clear();
    std::cout << "INFO: Unloaded all resources." << std::endl;
}

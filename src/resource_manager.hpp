#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "raylib.h"
#include <unordered_map>
#include <string>

class ResourceManager {
    std::unordered_map<std::string, Model> models_;
    std::unordered_map<std::string, Texture2D> textures_;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
public:
    ResourceManager();
    ~ResourceManager();

    Model load_model(const char* path);
    Texture2D load_texture(const char* path);

    void unload_all_resources();
};

#endif // RESOURCE_MANAGER_HPP

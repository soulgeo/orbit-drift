#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "raylib.h"
#include <memory>


class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    Model load_model(const char* path);
    Texture2D load_texture(const char* path);

    void unload_all_resources();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
};

#endif // RESOURCE_MANAGER_HPP

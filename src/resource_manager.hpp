#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "raylib.h"
#include <memory>

struct ResourceManagerImpl;

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    Model LoadModel(const char* path);
    Texture2D LoadTexture(const char* path);

    void UnloadAllResources();

private:
    std::unique_ptr<ResourceManagerImpl> impl_;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
};

#endif // RESOURCE_MANAGER_HPP

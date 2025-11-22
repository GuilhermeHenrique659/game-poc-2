#pragma once

#include <unordered_map>
#include "raylib.h"
#include <string>
#include <memory>

class ResourceManager
{
private:
    std::unordered_map<std::string, Texture2D> textures;

public:
    ResourceManager() = default;
    ~ResourceManager()
    {
        for (auto &pair : textures)
        {
            UnloadTexture(pair.second);
        }
    };

    static std::unique_ptr<ResourceManager> Create();
    void RegisterTexture(const std::string &key, const std::string &filePath);
    Texture2D &GetTexture(const std::string &key);
};
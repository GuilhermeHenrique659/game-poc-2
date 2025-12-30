#pragma once

#include <unordered_map>
#include "raylib.h"
#include <string>
#include <memory>
#include "../entity/common/Direction.h"
#include <tmxlite/Map.hpp>

class ResourceManager
{
private:
    std::unordered_map<std::string, Texture2D> textures;
    tmx::Map map;

    ResourceManager() {}
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ~ResourceManager()
    {
        for (auto &pair : textures)
        {
            UnloadTexture(pair.second);
        }
    };

public:
    static ResourceManager &Get();

    void RegisterTexture(const std::string &key, const std::string &filePath);
    Texture2D &GetTexture(const std::string &key);

    void AddMap(const std::string &filePath);
    tmx::Map &GetMap();
};
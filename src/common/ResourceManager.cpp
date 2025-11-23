#include "ResourceManager.h"

ResourceManager &ResourceManager::Get()
{
    static ResourceManager instance;

    return instance;
}

void ResourceManager::RegisterTexture(const std::string &key, const std::string &filePath)
{
    Texture2D texture = LoadTexture(filePath.c_str());
    textures[key] = texture;
}

Texture2D &ResourceManager::GetTexture(const std::string &key)
{
    return textures.at(key);
}
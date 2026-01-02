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
    if (!textures.count(key))
    {
        TraceLog(LOG_ERROR, "key not found: %s", key.c_str());
    }
    return textures.at(key);
}

void ResourceManager::AddMap(const std::string &filePath)
{
    if (!map.load(filePath))
    {
        TraceLog(LOG_FATAL, "Cannot load map: %s", filePath.c_str());
        return;
    };

    TraceLog(LOG_INFO, "Map version: %d.%d", map.getVersion().upper, map.getVersion().lower);

    TraceLog(LOG_INFO, "Map loaded successfully.");
}

tmx::Map &ResourceManager::GetMap()
{
    return map;
}
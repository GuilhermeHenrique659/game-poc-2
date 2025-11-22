#include "ResourceManager.h"

std::unique_ptr<ResourceManager> ResourceManager::Create()
{
    Image blankImage = GenImageColor(256, 256, BLANK);
    Texture2D emptyTexture = LoadTextureFromImage(blankImage);
    UnloadImage(blankImage);

    std::unique_ptr<ResourceManager> resourceManager = std::make_unique<ResourceManager>();

    resourceManager->textures["empty"] = emptyTexture;

    return resourceManager;
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
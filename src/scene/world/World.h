#pragma once
#include "../../common/Scene.h"
#include "../../player/Player.h"
#include "../../common/ResourceManager.h"
#include "raylib.h"

class World : public Scene
{
private:
    std::unique_ptr<Player> player;
    Camera2D camera = {0};
    std::unique_ptr<ResourceManager> resourceManager;

    /*     Texture2D background;
        Texture2D playerRunningTextureRight;
        Texture2D playerRunningTextureRightDown;
        Texture2D playerRunningTextureDown;
        Texture2D playerRunningTextureLeftDown;
        Texture2D playerRunningTextureLeft;
        Texture2D playerRunningTextureLeftUp;
        Texture2D playerRunningTextureUp;
        Texture2D playerRunningTextureRightUp; */

public:
    World(
        GameState &gameState, std::unique_ptr<ResourceManager> resourceManager) : Scene(gameState), resourceManager(std::move(resourceManager)) {};
    ~World() = default;

    void Setup();
    void Presenter(float delta);
    void Update(float delta);
};

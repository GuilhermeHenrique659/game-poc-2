#pragma once
#include "../../common/Scene.h"
#include "../../player/Player.h"
#include "raylib.h"

class World : public Scene
{
private:
    std::unique_ptr<Player> player;
    Camera2D camera = {0};
    Texture2D background;
    Texture2D playerRunningTextureRight;
    Texture2D playerRunningTextureRightDown;
    Texture2D playerRunningTextureDown;
    Texture2D playerRunningTextureLeftDown;
    Texture2D playerRunningTextureLeft;
    Texture2D playerRunningTextureLeftUp;
    Texture2D playerRunningTextureUp;
    Texture2D playerRunningTextureRightUp;

public:
    World(
        GameState &gameState,
        Texture2D background,
        Texture2D playerRunningTextureDown,
        Texture2D playerRunningTextureUp,
        Texture2D playerRunningTextureLeft,
        Texture2D playerRunningTextureRight,
        Texture2D playerRunningTextureLeftUp,
        Texture2D playerRunningTextureLeftDown,
        Texture2D playerRunningTextureRightUp,
        Texture2D playerRunningTextureRightDown) : Scene(gameState),
                                                   background(background),
                                                   playerRunningTextureDown(playerRunningTextureDown),
                                                   playerRunningTextureUp(playerRunningTextureUp),
                                                   playerRunningTextureLeft(playerRunningTextureLeft),
                                                   playerRunningTextureRight(playerRunningTextureRight),
                                                   playerRunningTextureLeftUp(playerRunningTextureLeftUp),
                                                   playerRunningTextureLeftDown(playerRunningTextureLeftDown),
                                                   playerRunningTextureRightUp(playerRunningTextureRightUp),
                                                   playerRunningTextureRightDown(playerRunningTextureRightDown)

    {};
    ~World() = default;

    void Setup();
    void Presenter(float delta);
    void Update(float delta);
};

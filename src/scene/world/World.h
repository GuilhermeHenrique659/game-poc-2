#pragma once
#include "../../common/Scene.h"
#include "../../entity/player/Player.h"
#include "../../common/ResourceManager.h"
#include "raylib.h"

class World : public Scene
{
private:
    std::unique_ptr<Player> player;
    Camera2D camera = {0};

public:
    World(
        GameState &gameState) : Scene(gameState) {};
    ~World() = default;

    void Setup();
    void Presenter(float delta);
    void Update(float delta);
};

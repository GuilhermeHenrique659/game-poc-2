#pragma once
#include "../../common/Scene.h"

class World : public Scene
{
private:
    /* data */
public:
    World(GameState &gameState) : Scene(gameState) {};
    ~World() = default;

    void Setup();
    void Presenter(float delta);
    void Update(float delta);
};

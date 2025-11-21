#pragma once
#include "GameState.h"

class Scene
{
protected:
    GameState &gameState;

public:
    Scene(GameState &gameState) : gameState(gameState) {};
    virtual ~Scene() = default;

    virtual void Setup() = 0;
    virtual void Presenter(float delta) = 0;
    virtual void Update(float delta) = 0;
};
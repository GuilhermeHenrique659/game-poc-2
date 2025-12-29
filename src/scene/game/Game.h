#pragma once

#include <memory>
#include "../../common/Scene.h"
#include "../../entity/EntityManager.h"
#include "../../view/ViewManager.h"

class Game : public Scene
{
private:
    std::unique_ptr<EntityManager> entity_manager;
    std::unique_ptr<ViewManager> view_manager;

public:
    Game(GameState &gameState) : Scene(gameState) {};
    ~Game() = default;

    void Setup() override;
    void Presenter(float delta) override;
    void Update(float delta) override;
};
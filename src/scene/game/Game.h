#pragma once

#include <memory>
#include "../../common/Scene.h"
#include "../../entity/EntityManager.h"
#include "../../view/ViewManager.h"
#include "../../components/Camera.cpp"

class Game : public Scene
{
private:
    std::shared_ptr<EntityManager> entity_manager;
    std::unique_ptr<ViewManager> view_manager;
    std::unique_ptr<CameraComponent> world_camera;

    uint32_t local_player_id;

public:
    Game(GameState &gameState) : Scene(gameState) {};
    ~Game() = default;

    void Setup() override;
    void Presenter(float delta) override;
    void Update(float delta) override;
};
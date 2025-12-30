#pragma once

#include <memory>
#include "../../common/Scene.h"
#include "../../entity/EntityManager.h"
#include "../../view/ViewManager.h"
#include "../../components/Camera.cpp"
#include "../../common/input/InputManager.h"

class Game : public Scene
{
private:
    std::shared_ptr<EntityManager> entity_manager;
    std::unique_ptr<ViewManager> view_manager;
    std::unique_ptr<CameraComponent> world_camera;
    std::unique_ptr<InputManager> input_manager;

    uint32_t local_player_id;

public:
    Game(GameState &gameState) : Scene(gameState), input_manager(std::make_unique<InputManager>()), entity_manager(std::make_shared<EntityManager>()), view_manager(std::make_unique<ViewManager>()) {};
    ~Game() = default;

    void Setup() override;
    void Presenter(float delta) override;
    void Update(float delta) override;
};
#pragma once

#include <memory>
#include "../../common/Scene.h"
#include "../../entity/EntityManager.h"
#include "../../view/ViewManager.h"
#include "../../components/Camera.cpp"
#include "../../common/input/InputManager.h"
#include "../../network/Network.h"
#include "GameMap.h"

class Game : public Scene
{
private:
    float light_flicker_timer;
    float light_flicker_interval;

    RenderTexture2D global_litght_texture;
    std::shared_ptr<EntityManager> entity_manager;
    std::shared_ptr<ViewManager> view_manager;
    std::unique_ptr<CameraComponent> world_camera;
    std::shared_ptr<InputManager> input_manager;
    std::unique_ptr<GameMap> map;
    std::shared_ptr<Network> network;

    uint32_t local_player_id;

    void BroadcastEntitiesSnapshot();
    void UpdateLighting(float delta, std::shared_ptr<Entity> player);

public:
    Game(GameState &gameState, std::shared_ptr<Network> network) : Scene(gameState),
                                                                   map(std::make_unique<GameMap>()),
                                                                   input_manager(std::make_shared<InputManager>()),
                                                                   entity_manager(std::make_shared<EntityManager>()),
                                                                   view_manager(std::make_shared<ViewManager>()),
                                                                   network(network),
                                                                   global_litght_texture(LoadRenderTexture(GetScreenWidth(), GetScreenHeight())),
                                                                   light_flicker_timer(0.0f),
                                                                   light_flicker_interval(1.0f) {};
    ~Game() = default;

    void Setup() override;
    void Presenter(float delta) override;
    void Update(float delta) override;
    void ConnectClient();
};
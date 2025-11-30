#pragma once
#include "../../common/Scene.h"
#include "../../entity/player/Player.h"
#include "../../common/ResourceManager.h"
#include "../../entity/EntityManager.h"
#include "raylib.h"

class World : public Scene
{
private:
    std::shared_ptr<Player> player;
    Camera2D camera = {0};
    EntityManager *entityManager;
    Network *network;

public:
    bool isHost = false;

    World(
        GameState &gameState, EntityManager *entityManager, Network *network) : Scene(gameState), entityManager(entityManager), network(network) {};
    ~World() = default;

    void StartHost();
    void ConnectToHost();

    void Setup();
    void Presenter(float delta);
    void Update(float delta);
};

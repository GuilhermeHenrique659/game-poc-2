#pragma once
#include "../../common/Scene.h"
#include "../../entity/player/Player.h"
#include "../../common/ResourceManager.h"
#include "../../network/Network.h"
#include "raylib.h"

struct PlayerMoved
{
    uint32_t id;
    Vector2 position;
    PlayerDirection direction;
    bool isIdle;
    float angle;
};

class World : public Scene
{
private:
    std::unique_ptr<Player> player;
    Camera2D camera = {0};

public:
    Network *network;
    std::unordered_map<uint32_t, PlayerMoved> remotePlayers;
    std::unique_ptr<Player> remotePlayer;

    uint32_t myPlayerId = 0;
    bool isHost = false;

    World(
        GameState &gameState, Network *network) : Scene(gameState), network(network) {};
    ~World() = default;

    void StartHost();
    void ConnectToHost();

    void Setup();
    void Presenter(float delta);
    void Update(float delta);
};

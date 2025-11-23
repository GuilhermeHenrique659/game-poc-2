#pragma once
#include "../../common/Scene.h"
#include "../../entity/player/Player.h"
#include "../../common/ResourceManager.h"
#include "../../network/Network.h"
#include "raylib.h"

class World : public Scene
{
private:
    std::unique_ptr<Player> player;
    Camera2D camera = {0};

public:
    Network network;
    std::unordered_map<uint32_t, RemotePlayer> remotePlayers;
    std::unique_ptr<Player> remotePlayer;

    uint32_t myPlayerId = 0;
    bool isHost = false;

    World(
        GameState &gameState) : Scene(gameState) {};
    ~World() = default;

    void StartHost();
    void ConnectToHost();

    void Setup();
    void Presenter(float delta);
    void Update(float delta);
};

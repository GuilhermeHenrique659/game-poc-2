#pragma once
#include <raylib_win.h>
#include <raylib.h>
#include <enet/enet.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

#include "../entity/player/PlayerDirection.h"

struct RemotePlayer
{
    uint32_t id;
    Vector2 position;
    PlayerDirection direction;
    bool isIdle;
    float angle;
    ENetPeer *peer = nullptr;
    bool isLocal;
};

class Network
{
public:
    bool isServer = false;
    ENetHost *host = nullptr;
    ENetAddress address;
    ENetEvent event;

    // Apenas no server
    std::unordered_map<uint32_t, RemotePlayer> players;
    uint32_t nextPlayerId = 1;
    uint32_t localPlayerId = 0;

    bool InitAsServer(uint16_t port = 5000);
    bool InitAsClient(const std::string &ip = "127.0.0.1", uint16_t port = 5000);
    void Update(float delta);
    void SendMyState(const Vector2 &pos, PlayerDirection dir, bool idle, float angle);
    void BroadcastMyState(const Vector2 &pos, PlayerDirection dir, bool idle, float angle);
    void Shutdown();
};
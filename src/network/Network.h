#pragma once
#include <raylib_win.h>

#include <enet/enet.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "NetworkClient.h"

class Network
{
private:
    std::shared_ptr<NetworkClient> networkClient = std::make_shared<NetworkClient>();

public:
    bool isServer = false;
    ENetHost *host = nullptr;
    ENetAddress address;
    ENetEvent event;

    bool InitAsServer(uint16_t port = 5000);
    bool InitAsClient(const std::string &ip = "127.0.0.1", uint16_t port = 5000);
    void Update();
    void Send(RemotePacket *remotePacket, size_t dataSize);
    void Shutdown();

    void addListner(const std::string &event, std::unique_ptr<Observer> observer);
};
#pragma once
#include "../network/Network.h"
#include "player/Player.h"
#include "player/PlayerDto.h"
#include "../network/Events.h"

class EntityManager
{
private:
    Network *network;

    std::unordered_map<uint32_t, std::shared_ptr<Player>> players;

public:
    EntityManager(Network *network) : network(network) {}
    ~EntityManager() = default;

    std::shared_ptr<Player> getPlayer(uint32_t id);
    void createPlayer(Vector2 position);
    void broadcastPlayer(EventName event, PlayerDto player);
};

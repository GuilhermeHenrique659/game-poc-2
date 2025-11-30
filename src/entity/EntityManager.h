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
    uint32_t currentPlayerId = 0;

    EntityManager(Network *network) : network(network) {}
    ~EntityManager() = default;

    std::shared_ptr<Player> getPlayer(uint32_t id);
    uint32_t createPlayer(Vector2 position, uint32_t id);
    void updatePlayer(PlayerDto player);

    void broadcastPlayer(EventName event, PlayerDto player);

    void addListner(const std::string &event, std::unique_ptr<Observer> observer);

    std::unordered_map<uint32_t, std::shared_ptr<Player>> getPlayers();
};

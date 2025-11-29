#include "EntityManager.h"

std::shared_ptr<Player> EntityManager::getPlayer(uint32_t id)
{
    if (players.empty())
        return;

    auto it = players.find(id);
    if (it == players.end())
        return;

    if (it != players.end())
    {
        return it->second;
    }
}

void EntityManager::createPlayer(Vector2 position)
{
    Rectangle destRec = {
        position.x,
        position.y,
        256.0f,
        256.0f};

    auto player = std::make_shared<Player>(
        position,
        destRec,
        PlayerSpriteAnimation());

    uint32_t newId = players.size() + 1;

    players[newId] = player;
}

void EntityManager::broadcastPlayer(EventName event, PlayerDto player)
{
    const size_t dataSize = sizeof(PlayerDto);

    RemotePacket packet{};

    packet.eventId = EVENT_DEFINITIONS[event].id;

    memcpy(packet.data, &player, dataSize);

    network->Send(&packet, dataSize);
}
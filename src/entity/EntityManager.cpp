#include "EntityManager.h"
#include "../config.h"

std::shared_ptr<Player> EntityManager::getPlayer(uint32_t id)
{
    return players[id];
}

std::unordered_map<uint32_t, std::shared_ptr<Enemy>> EntityManager::getEnemies()
{
    if (enemies.empty())
        createEnemy({0, 0}, 0);

    return enemies;
}

void EntityManager::killEnemy(uint32_t id)
{
    enemies.erase(id);
}

std::unordered_map<uint32_t, std::shared_ptr<Player>> EntityManager::getPlayers()
{
    return players;
}

void EntityManager::updatePlayer(PlayerDto data)
{
    if (players.find(data.id) == players.end())
    {
        createPlayer({0, 0}, data.id);
    }

    auto player = players[data.id];

    player->UpdatePosition(data.position);
    player->SetPlayerDirection(static_cast<PlayerDirection>(data.direction));
    player->SetPlayerState(data.state);
}

uint32_t EntityManager::createEnemy(Vector2 position, uint32_t id)
{
    Rectangle destRec = {
        position.x,
        position.y,
        SPRITE_FRAME_WIDHT,
        SPRITEH_FRAME_HEIGHT};

    auto enemy = std::make_shared<Enemy>(
        position,
        destRec,
        EnemySpriteAnimation());

    uint32_t newId;
    TraceLog(LOG_INFO, "id passed: %d", id);

    if (id > 0)
    {
        newId = id;
    }
    else
    {
        newId = enemies.size() + 1;
    }

    TraceLog(LOG_INFO, "Enemy Create with id: %d", newId);
    enemies[newId] = enemy;

    return newId;
}

uint32_t EntityManager::createPlayer(Vector2 position, uint32_t id)
{
    Rectangle destRec = {
        position.x,
        position.y,
        SPRITE_FRAME_WIDHT,
        SPRITEH_FRAME_HEIGHT};

    auto player = std::make_shared<Player>(
        position,
        destRec,
        PlayerSpriteAnimation());

    uint32_t newId;
    TraceLog(LOG_INFO, "id passed: %d", id);

    if (id > 0)
    {
        newId = id;
    }
    else
    {
        newId = players.size() + 1;
    }

    TraceLog(LOG_INFO, "Player Create with id: %d", newId);
    players[newId] = player;

    return newId;
}

void EntityManager::broadcastPlayer(EventName event, PlayerDto player)
{
    const size_t dataSize = sizeof(PlayerDto);

    RemotePacket packet{};

    packet.eventId = EVENT_DEFINITIONS[event].id;
    memcpy(packet.data, &player, dataSize);

    network->Send(&packet, dataSize);
}

void EntityManager::addListner(const std::string &event, std::unique_ptr<Observer> observer)
{
    network->addListner(event, std::move(observer));
}
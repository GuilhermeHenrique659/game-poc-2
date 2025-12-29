#pragma once
#include "../network/Network.h"
#include "player/Player.h"
#include "player/PlayerDto.h"
#include "enemy/Enemy.h"
#include "enemy/EnemyDto.h"
#include "../network/Events.h"
#include "../view/EntityAnimationManager.h"

struct EntityState
{
    uint32_t id;
    Vector2 position;
    int state;
    int direction;
    int health;
};

struct SnapshotState
{
    std::vector<EntityState> players;
    std::vector<EntityState> enemies;

    std::vector<uint8_t> serialize() const;
    static SnapshotState deserialize(const uint8_t *data, size_t size);
};

class EntityManager
{
private:
    Network *network;
    EntityAnimationManager *animationManager;

    std::unordered_map<uint32_t, std::shared_ptr<Player>> players;
    std::unordered_map<uint32_t, std::shared_ptr<Enemy>> enemies;

public:
    uint32_t currentPlayerId = 0;
    bool remoteIdSettled = false;

    float accumulator = 0;
    const float SNAP_RATE = 0.016f;

    EntityManager(Network *network);
    ~EntityManager() = default;

    std::shared_ptr<Player> getPlayer(uint32_t id);
    uint32_t createPlayer(Vector2 position, uint32_t id);
    uint32_t createEnemy(Vector2 position, uint32_t id);

    void updateEnemy(EnemyDto player);
    void updatePlayer(PlayerDto player);

    void broadcastPlayer(EventName event, PlayerDto player);
    void broadcastSnapshot();

    void addListner(const std::string &event, std::unique_ptr<Observer> observer);

    void killEnemy(uint32_t id);

    std::unordered_map<uint32_t, std::shared_ptr<Player>> getPlayers();
    std::unordered_map<uint32_t, std::shared_ptr<Enemy>> getEnemies();

    EntityAnimationManager *getAnimationManager() { return animationManager; }
};

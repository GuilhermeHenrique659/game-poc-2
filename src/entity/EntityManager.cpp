#include "EntityManager.h"
#include "../config.h"
#include "../common/Subject.h"
#include "../network/Events.h"
#include "../network/Package.h"

std::vector<uint8_t> SnapshotState::serialize() const
{
    std::vector<uint8_t> out;

    auto append = [&](auto v)
    {
        size_t pos = out.size();
        out.resize(pos + sizeof(v));
        memcpy(out.data() + pos, &v, sizeof(v));
    };

    uint32_t pc = players.size();
    uint32_t ec = enemies.size();

    append(pc);
    append(ec);

    for (auto &p : players)
    {
        append(p.id);
        append(p.position);
        append(p.direction);
    }

    for (auto &e : enemies)
    {
        append(e.id);
        append(e.position);
        append(e.direction);
    }

    return out;
}

SnapshotState SnapshotState::deserialize(const uint8_t *data, size_t size)
{
    SnapshotState s;

    auto read = [&](auto &v)
    {
        memcpy(&v, data, sizeof(v));
        data += sizeof(v);
    };

    uint32_t pc, ec;
    read(pc);
    read(ec);

    s.players.resize(pc);
    s.enemies.resize(ec);

    for (auto &p : s.players)
    {
        read(p.id);
        read(p.position);
        read(p.direction);
    }

    for (auto &e : s.enemies)
    {
        read(e.id);
        read(e.position);
        read(e.direction);
    }

    return s;
}

class SnapshotObserver : public Observer
{
private:
    EntityManager *entities;

public:
    SnapshotObserver(EntityManager *em) : entities(em) {}

    void notify(const std::any &data) override
    {
        RemotePacket pkg = std::any_cast<RemotePacket>(data);
        const uint8_t *pm{};
        memcpy(&pm, pkg.data, sizeof(pkg.data));

        SnapshotState snap =
            SnapshotState::deserialize(pm, sizeof(pkg.data));

        // atualizar players
        for (auto &state : snap.players)
        {
            if (state.id != entities->currentPlayerId)
                entities->updatePlayer({
                    .id = state.id,
                    .position = state.position,
                    .direction = (Direction)state.direction,
                    .state = (PlayerState)state.state,
                });
        }

        // atualizar enemies
        for (auto &state : snap.enemies)
        {
            entities->updateEnemy({
                .id = state.id,
                .position = state.position,
                .direction = (Direction)state.direction,
                .state = (EnemyState)state.state,
            });
        }
    }
};

EntityManager::EntityManager(Network *network) : network(network), animationManager(new EntityAnimationManager())
{
    addListner("world_snapshot", std::make_unique<SnapshotObserver>(this));
}

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
    player->SetDirection(static_cast<Direction>(data.direction));
    player->SetPlayerState(data.state);
}

void EntityManager::updateEnemy(EnemyDto data)
{
    if (enemies.find(data.id) == enemies.end())
    {
        createPlayer({0, 0}, data.id);
    }

    auto enemy = enemies[data.id];

    enemy->UpdatePosition(data.position);
    enemy->SetDirection(data.direction);
    enemy->SetEnemyState(data.state);
}

uint32_t EntityManager::createEnemy(Vector2 position, uint32_t id)
{
    Rectangle destRec = {
        position.x,
        position.y,
        SPRITE_FRAME_WIDHT,
        SPRITEH_FRAME_HEIGHT};

    uint32_t newId;
    TraceLog(LOG_INFO, "id passed: %d", id);

    if (id > 0)
    {
        newId = id;
    }
    else
    {
        newId = enemies.size() + players.size() + 1;
    }

    auto enemy = std::make_shared<Enemy>(
        newId,
        position,
        destRec);

    TraceLog(LOG_INFO, "Enemy Create with id: %d", newId);
    enemies[newId] = enemy;

    animationManager->AddSprite(enemy);

    return newId;
}

uint32_t EntityManager::createPlayer(Vector2 position, uint32_t id)
{
    Rectangle destRec = {
        position.x,
        position.y,
        SPRITE_FRAME_WIDHT,
        SPRITEH_FRAME_HEIGHT};

    uint32_t newId;
    TraceLog(LOG_INFO, "id passed: %d", id);

    if (id > 0)
    {
        newId = id;
    }
    else
    {
        newId = enemies.size() + players.size() + 1;
    }

    auto player = std::make_shared<Player>(
        newId,
        position,
        destRec);

    TraceLog(LOG_INFO, "Player Create with id: %d", newId);
    players[newId] = player;

    animationManager->AddSprite(player);

    return newId;
}

void EntityManager::broadcastSnapshot()
{
    SnapshotState snap;

    // players
    for (auto &[id, p] : players)
    {
        snap.players.push_back(EntityState{
            .id = id,
            .position = p->GetPosition(),
            .state = p->GetPlayerState(),
            .direction = (int)p->GetEntityDirection(),
            .health = p->health,
        });
    }

    // enemies
    for (auto &[id, e] : enemies)
    {
        snap.players.push_back(EntityState{
            .id = id,
            .position = e->GetPosition(),
            .state = (int)e->GetEnemyState(),
            .direction = (int)e->GetEntityDirection(),
            .health = e->health,
        });
    }

    RemotePacket pkt;

    pkt.eventId = EVENT_DEFINITIONS[EventName::WORLD_SNAPSHOT].id;
    auto data = snap.serialize();
    memcpy(pkt.data, &data, sizeof(data));

    network->Send(&pkt, sizeof(data));
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
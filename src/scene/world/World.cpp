#include "world.h"
#include "raymath.h"
#include "../../entity/player/PlayerSpriteAnimation.h"
#include "../../common/util/VectorUtil.h"
#include "../../network/Events.h"
#include "../../network/Package.h"
#include "../../config.h"

class PlayerAttacks : public Observer
{
private:
    EntityManager *entityManager;

public:
    PlayerAttacks(EntityManager *entityManager) : entityManager(entityManager) {}

    void notify(const std::any &data)
    {
        TraceLog(LOG_INFO, "player attacks");
        auto attackBox = std::any_cast<std::optional<Rectangle>>(data);

        for (auto &[id, e] : entityManager->getEnemies())
        {
            if (attackBox.has_value())
                e->OnHit(attackBox.value());
        }
    }
};

class PlayerMovedObserver : public Observer
{
private:
    EntityManager *entityManager;

public:
    PlayerMovedObserver(EntityManager *entityManager) : entityManager(entityManager) {}

    void notify(const std::any &data)
    {
        RemotePacket pkg = std::any_cast<RemotePacket>(data);
        PlayerDto pm{};
        memcpy(&pm, pkg.data, sizeof(PlayerDto));

        if (pm.id != entityManager->currentPlayerId)
        {
            entityManager->updatePlayer(pm);
        }
    }
};

class PlayerHittedObserver : public Observer
{
private:
    EntityManager *entityManager;

public:
    PlayerHittedObserver(EntityManager *entityManager) : entityManager(entityManager) {}

    void notify(const std::any &data)
    {
        RemotePacket pkg = std::any_cast<RemotePacket>(data);
        PlayerDto pm{};
        memcpy(&pm, pkg.data, sizeof(PlayerDto));

        if (pm.id == entityManager->currentPlayerId)
        {
            TraceLog(LOG_INFO, "hitted");
        }
    }
};

class OnConnected : public Observer
{
private:
    EntityManager *entityManager;

public:
    OnConnected(EntityManager *entityManager) : entityManager(entityManager) {}

    void notify(const std::any &data)
    {
        uint32_t clientId = entityManager->createPlayer({0, 0}, 0);

        std::shared_ptr<Player> clientPlayer = entityManager->getPlayer(clientId);

        PlayerDto clientDto = {
            .id = clientId,
            .position = clientPlayer->GetPosition(),
            .direction = clientPlayer->GetPlayerDirection(),
            .state = clientPlayer->GetPlayerState(),
        };

        TraceLog(LOG_INFO, "Host assigned ID %d to new client", clientId);
        entityManager->broadcastPlayer(EventName::ID_ASSIGNEED, clientDto);
    }
};

class IdAssigneed : public Observer
{
private:
    EntityManager *entityManager;

public:
    IdAssigneed(EntityManager *entityManager) : entityManager(entityManager) {}

    void notify(const std::any &data)
    {
        RemotePacket pkg = std::any_cast<RemotePacket>(data);
        PlayerDto pm{};
        memcpy(&pm, pkg.data, sizeof(PlayerDto));

        if (entityManager->remoteIdSettled)
            return;

        entityManager->getPlayers().erase(entityManager->currentPlayerId);
        entityManager->currentPlayerId = pm.id;

        entityManager->createPlayer(pm.position, entityManager->currentPlayerId);

        entityManager->remoteIdSettled = true;
        TraceLog(LOG_INFO, "Client: Remove id settled changed currentId to %d", pm.id);
    }
};

void World::Setup()
{
    Vector2 startPos = IsoWorldToScreen(GetScreenWidth() / 2.0f / SPRITE_FRAME_WIDHT, GetScreenHeight() / 2.0f / SPRITE_FRAME_WIDHT);

    entityManager->createEnemy({900, 900}, 0);

    auto playerId = entityManager->createPlayer(startPos, 0);
    entityManager->currentPlayerId = playerId;
    auto player = entityManager->getPlayer(playerId);

    entityManager->addListner("player_moved", std::make_unique<PlayerMovedObserver>(entityManager));
    entityManager->addListner("hitted", std::make_unique<PlayerHittedObserver>(entityManager));
    player->subscribe("player_attacked", std::make_unique<PlayerAttacks>(entityManager));

    camera = CameraComponent::create(player->GetPosition(), player->GetDestReactangle());
}

void World::Update(float delta)
{
    std::shared_ptr<Player> player = entityManager->getPlayer(entityManager->currentPlayerId);
    std::vector<std::shared_ptr<Player>> players;

    std::vector<Rectangle> collisionRectangles;

    camera->update(player->GetPosition(), player->GetDestReactangle());

    for (const auto &pair : entityManager->getPlayers())
    {
        if (player->IsHitted(pair.second->GetCollisionRectangle()))
            entityManager->broadcastPlayer(EventName::HITTED, {
                                                                  .id = pair.first,
                                                                  .position = pair.second->GetPosition(),
                                                                  .direction = pair.second->GetPlayerDirection(),
                                                                  .state = pair.second->GetPlayerState(),
                                                              });
        players.push_back(pair.second);

        if (pair.first == entityManager->currentPlayerId)
            continue;

        collisionRectangles.push_back(pair.second->GetCollisionRectangle());
    }

    player->Attack();

    if (player->GetPlayerState() != ATTACK)
        player->move(collisionRectangles);
    for (const auto &[id, e] : entityManager->getEnemies())
    {
        if (e->health == 0)
        {
            entityManager->killEnemy(id);
        }
        e->move(collisionRectangles, players);
    }

    static float sendTimer = 0;

    if (IsKeyPressed(KEY_H))
    {

        TraceLog(LOG_INFO, "Start as host");
        StartHost(); // tecla H = host

        entityManager->addListner("connection", std::make_unique<OnConnected>(entityManager));
    }
    if (IsKeyPressed(KEY_C))
    {
        TraceLog(LOG_INFO, "Try connect");
        ConnectToHost(); // tecla C = conectar em 127.0.0.1

        entityManager->addListner("id_assign", std::make_unique<IdAssigneed>(entityManager));
    }
}

void World::ConnectToHost()
{
    if (network->InitAsClient())
    {
        TraceLog(LOG_INFO, "Connect with success");
    };
}

void World::StartHost()
{
    network->InitAsServer();
}

void World::Presenter(float delta)
{
    if (network->isServer)
    {
        DrawText("HOSTING SERVER", 100, 100, 12, BLACK);
    }

    std::shared_ptr<Player> player = entityManager->getPlayer(entityManager->currentPlayerId);

    BeginMode2D(camera->getCamera());
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            Vector2 gridPostion = IsoWorldToScreen(i, j);
            Texture2D background = ResourceManager::Get().GetTexture("floor");

            DrawTexturePro(
                background,
                {0.0f, 0.0f, (float)background.width, (float)background.height},
                {(float)(gridPostion.x), (float)(gridPostion.y), (float)background.width, (float)background.height},
                {0, 0},
                0.0f,
                WHITE);
        }
    }

    player->Animate();
    PlayerSpriteAnimation sprite = player->GetPlayerSprite();

    DrawTexturePro(
        sprite.GetCurrentTexture(),
        sprite.GetSourceRectangle(),
        player->GetDestReactangle(),
        {0, 0},
        0.0f,
        WHITE);

    if (!entityManager->getPlayers().empty())
    {
        for (auto &[id, rp] : entityManager->getPlayers())
        {
            if (!network->isServer && id == entityManager->currentPlayerId)
                continue;

            if (network->isServer && id == entityManager->currentPlayerId)
                continue;

            rp->Animate();
            PlayerSpriteAnimation remoteSprite = rp->GetPlayerSprite();

            DrawTexturePro(
                remoteSprite.GetCurrentTexture(),
                remoteSprite.GetSourceRectangle(),
                rp->GetDestReactangle(),
                {0, 0},
                0,
                Fade(WHITE, 0.8f));
        }
    }

    if (!entityManager->getEnemies().empty())
    {
        for (auto &[id, rp] : entityManager->getEnemies())
        {
            rp->Animate();
            auto remoteSprite = rp->GetEnemySprite();

            DrawTexturePro(
                remoteSprite.GetCurrentTexture(),
                remoteSprite.GetSourceRectangle(),
                rp->GetDestReactangle(),
                {0, 0},
                0,
                Fade(RED, 0.8f));
        }
    }

    EndMode2D();

    DrawText(TextFormat("Vida %d/10", player->health), 10, 30, 12, GREEN);
}

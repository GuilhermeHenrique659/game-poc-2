#include "world.h"
#include "raymath.h"
#include "../../entity/player/PlayerSpriteAnimation.h"
#include "../../common/util/VectorUtil.h"
#include "../../network/Events.h"
#include "../../network/Package.h"

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
            .position = clientPlayer->position,
            .direction = clientPlayer->GetPlayerDirection(),
            .isIdle = clientPlayer->isIdle,
            .angle = clientPlayer->angle};

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

        entityManager->getPlayers().erase(entityManager->currentPlayerId);
        entityManager->getPlayers().erase(pm.id);

        entityManager->currentPlayerId = pm.id;

        TraceLog(LOG_INFO, "Client: Id assigneed to %d", pm.id);

        auto createId = entityManager->createPlayer(pm.position, entityManager->currentPlayerId);

        TraceLog(LOG_INFO, "Create: %d", createId);
        TraceLog(LOG_INFO, "Client: myPlayerId changed to %d", pm.id);
    }
};

void World::Setup()
{
    Vector2 startPos = IsoWorldToScreen(GetScreenWidth() / 2.0f / 256.0f, GetScreenHeight() / 2.0f / 128.0f);

    auto playerId = entityManager->createPlayer(startPos, 0);
    entityManager->currentPlayerId = playerId;

    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.offset = {(float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f - 128.0f};
    camera.target = entityManager->getPlayer(playerId)->position;

    entityManager->addListner("player_moved", std::make_unique<PlayerMovedObserver>(entityManager));
}

void World::Update(float delta)
{
    std::shared_ptr<Player> player = entityManager->getPlayer(entityManager->currentPlayerId);
    player->move(camera);
    camera.target = Vector2Lerp(camera.target, player->position, 5.0f * delta);

    camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));

    if (camera.zoom > 3.0f)
        camera.zoom = 3.0f;
    else if (camera.zoom < 0.1f)
        camera.zoom = 0.1f;

    static float sendTimer = 0;
    sendTimer += delta;

    if (sendTimer > 1.0f / 40.0f)
    {
        entityManager->broadcastPlayer(EventName::PLAYER_MOVED, {.id = entityManager->currentPlayerId,
                                                                 .position = player->position,
                                                                 .direction = player->GetPlayerDirection(),
                                                                 .isIdle = player->isIdle,
                                                                 .angle = player->angle});
        sendTimer = 0;
    }

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

    BeginMode2D(camera);
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
        player->destRec,
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
                rp->destRec,
                {128, 128},
                0,
                Fade(RED, 0.8f));
        }
    }

    EndMode2D();
}

#include "world.h"
#include "raymath.h"
#include "../../entity/player/PlayerSpriteAnimation.h"
#include "../../common/util/VectorUtil.h"
#include "../../network/Events.h"
#include "../../network/Package.h"

class PlayerMovedObserver : public Observer
{
private:
    std::unordered_map<uint32_t, PlayerMoved> &remotePlayers;

public:
    PlayerMovedObserver(std::unordered_map<uint32_t, PlayerMoved> &remotePlayers) : remotePlayers(remotePlayers) {}

    void notify(const std::any &data)
    {
        RemotePacket pkg = std::any_cast<RemotePacket>(data);
        PlayerMoved pm{};
        memcpy(&pm, pkg.data, sizeof(PlayerMoved));

        remotePlayers[pm.id] = pm;
    }
};

class OnConnected : public Observer
{
private:
    std::unordered_map<uint32_t, PlayerMoved> &remotePlayers;
    Network *network;

public:
    OnConnected(std::unordered_map<uint32_t, PlayerMoved> &remotePlayers, Network *network) : remotePlayers(remotePlayers), network(network) {}

    void notify(const std::any &data)
    {
        uint32_t newId = remotePlayers.size() + 1;

        remotePlayers[newId] = {
            .id = newId,
            .position = Vector2{0, 0},
            .direction = DOWN,
            .isIdle = true,
            .angle = 0};

        const size_t dataSize = sizeof(PlayerMoved);

        RemotePacket packet{};
        packet.eventId = 1;

        memcpy(packet.data, &remotePlayers[2], dataSize);

        network->Send(&packet, dataSize);
    }
};

class IdAssigneed : public Observer
{
private:
    std::unordered_map<uint32_t, PlayerMoved> &remotePlayers;

public:
    IdAssigneed(std::unordered_map<uint32_t, PlayerMoved> &remotePlayers) : remotePlayers(remotePlayers) {}

    void notify(const std::any &data)
    {
        RemotePacket pkg = std::any_cast<RemotePacket>(data);
        PlayerMoved pm{};
        memcpy(&pm, pkg.data, sizeof(PlayerMoved));

        TraceLog(LOG_INFO, "Id assigneed: %d", pm.id);

        remotePlayers[pm.id] = pm;
    }
};

void World::Setup()
{
    Vector2 startPos = IsoWorldToScreen(GetScreenWidth() / 2.0f / 256.0f, GetScreenHeight() / 2.0f / 128.0f);

    Rectangle destRec = {
        startPos.x,
        startPos.y,
        256.0f,
        256.0f};

    player = std::make_unique<Player>(
        startPos,
        destRec,
        PlayerSpriteAnimation());

    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.offset = {(float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f - 128.0f};
    camera.target = player->position;

    network->addListner("player_moved", std::make_unique<PlayerMovedObserver>(remotePlayers));
}

void World::Update(float delta)
{
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
        PlayerMoved p = {
            .id = network->isServer ? 1 : 2,
            .position = player->position,
            .direction = player->GetPlayerDirection(),
            .isIdle = player->isIdle,
            .angle = player->angle};

        const size_t dataSize = sizeof(PlayerMoved);

        RemotePacket packet{};
        packet.eventId = 1;

        memcpy(packet.data, &p, dataSize);

        network->Send(&packet, dataSize);
        sendTimer = 0;
    }

    if (IsKeyPressed(KEY_H))
    {

        TraceLog(LOG_INFO, "Start as host");
        StartHost(); // tecla H = host

        network->addListner("connection", std::make_unique<OnConnected>(remotePlayers, network));
    }
    if (IsKeyPressed(KEY_C))
    {
        TraceLog(LOG_INFO, "Try connect");
        ConnectToHost(); // tecla C = conectar em 127.0.0.1

        network->addListner("id_assign", std::make_unique<IdAssigneed>(remotePlayers));
    }

    if (!network->isServer)
    {
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

    if (!remotePlayers.empty())
    {
        uint32_t ownId = network->isServer ? 1u : 2u;

        for (auto &[id, rp] : remotePlayers)
        {
            if (!network->isServer && id == ownId)
                continue;

            Rectangle dest = {rp.position.x, rp.position.y, 256, 256};

            if (!remotePlayer)
            {
                remotePlayer = std::make_unique<Player>(rp.position, dest, PlayerSpriteAnimation());
            }

            remotePlayer->position = rp.position;
            remotePlayer->destRec = dest;
            remotePlayer->SetPlayerDirection(static_cast<PlayerDirection>(rp.direction));
            remotePlayer->angle = rp.angle;
            remotePlayer->isIdle = rp.isIdle;

            remotePlayer->Animate();
            PlayerSpriteAnimation remoteSprite = remotePlayer->GetPlayerSprite();

            DrawTexturePro(
                remoteSprite.GetCurrentTexture(),
                remoteSprite.GetSourceRectangle(),
                dest,
                {128, 128},
                0,
                Fade(RED, 0.8f));
        }
    }

    EndMode2D();
}

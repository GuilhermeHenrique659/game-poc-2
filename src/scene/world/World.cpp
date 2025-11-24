#include "world.h"
#include "raymath.h"
#include "../../entity/player/PlayerSpriteAnimation.h"
#include "../../common/util/VectorUtil.h"

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
}

void World::Update(float delta)
{
    network.Update(delta);

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
        network.SendMyState(
            player->position,
            player->GetPlayerDirection(),
            player->isIdle,
            player->angle);
        sendTimer = 0;
    }

    if (IsKeyPressed(KEY_H))
    {
        TraceLog(LOG_INFO, "Start as host");
        StartHost(); // tecla H = host
    }
    if (IsKeyPressed(KEY_C))
    {
        TraceLog(LOG_INFO, "Try connect");
        ConnectToHost(); // tecla C = conectar em 127.0.0.1
    }

    // Atualiza remote players (apenas cliente)
    if (!network.isServer)
    {
        // Copia do server (você pode melhorar com interpolação depois)
        // por enquanto vamos ignorar ou guardar em remotePlayers
    }
}

void World::ConnectToHost()
{
    if (network.InitAsClient())
    {
        TraceLog(LOG_INFO, "Connect with success");
    };
}

void World::StartHost()
{
    network.InitAsServer();
}

void World::Presenter(float delta)
{
    if (network.isServer)
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

    // Desenha o player local
    player->Animate();
    PlayerSpriteAnimation sprite = player->GetPlayerSprite();

    DrawTexturePro(
        sprite.GetCurrentTexture(),
        sprite.GetSourceRectangle(),
        player->destRec,
        {0, 0},
        0.0f,
        WHITE);

    // Desenha todos os players conhecidos pela rede
    for (auto &[id, rp] : network.players)
    {
        // No cliente: não desenha o próprio player de novo
        if (!network.isServer && id == network.myPlayerId)
            continue;

        // No servidor: se quiser, também pode pular o id == myPlayerId (0) e desenhar só clientes
        // if (network.isServer && id == network.myPlayerId) continue;

        Rectangle dest = {rp.position.x, rp.position.y, 256, 256};

        if (remotePlayer == NULL)
        {
            remotePlayer = std::make_unique<Player>(rp.position, dest, PlayerSpriteAnimation());
        }

        remotePlayer->position = rp.position;
        remotePlayer->destRec = dest;
        remotePlayer->SetPlayerDirection(rp.direction);
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
            Fade(BLACK, 0.8f));
    }

    EndMode2D();
}

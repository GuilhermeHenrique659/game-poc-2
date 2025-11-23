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
    player->move(camera);
    camera.target = Vector2Lerp(camera.target, player->position, 5.0f * delta);

    camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));

    if (camera.zoom > 3.0f)
        camera.zoom = 3.0f;
    else if (camera.zoom < 0.1f)
        camera.zoom = 0.1f;
}

void World::Presenter(float delta)
{
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
    /*
        if (player->isIdle)
        {
            DrawTexturePro(
                player->texture,
                {(float)player->sourceRec.width * dir, 0.0f, (float)player->sourceRec.width, (float)player->sourceRec.height},
                player->destRec,
                {0, 0},
                0.0f,
                WHITE);
        }
        else
        {
            Texture2D currentTexture;

            switch (dir)
            {
            case 0:
                currentTexture = resourceManager->GetTexture("run_right");
                break;
            case 1:
                currentTexture = resourceManager->GetTexture("run_right_down");
                break;
            case 2:
                currentTexture = resourceManager->GetTexture("run_down");
                break;
            case 3:
                currentTexture = resourceManager->GetTexture("run_left_down");
                break;
            case 4:
                currentTexture = resourceManager->GetTexture("run_left");
                break;
            case 5:
                currentTexture = resourceManager->GetTexture("run_left_up");
                break;
            case 6:
                currentTexture = resourceManager->GetTexture("run_up");
                break;
            case 7:
                currentTexture = resourceManager->GetTexture("run_right_up");
                break;
            default:
                currentTexture = resourceManager->GetTexture("run_down");
                break;
            }

            DrawTexturePro(
                currentTexture,
                {(float)player->sourceRec.width * player->currentFrame, 0.0f, (float)player->sourceRec.width, (float)player->sourceRec.height},
                player->destRec,
                {0, 0},
                0.0f,
                WHITE);
        } */

    EndMode2D();
}

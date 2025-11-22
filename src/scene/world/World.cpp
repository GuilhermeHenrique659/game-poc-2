#include "world.h"
#include "raymath.h"

Vector2 IsoWorldToScreen(float x, float y)
{
    return (Vector2){(x - y) * (256 / 2.0f), (x + y) * (128 / 2.0f)};
}

void World::Setup()
{

    background = LoadTexture("resources/sprites/floor.png");

    Texture2D texture = LoadTexture("resources/sprites/idle_sheet.png");

    Vector2 startPos = IsoWorldToScreen(GetScreenWidth() / 2.0f / 128.0f, GetScreenHeight() / 2.0f / 64.0f);

    Rectangle destRec = {
        startPos.x,
        startPos.y,
        256.0f,
        256.0f};

    destRec.x -= destRec.width * 0.5f;
    destRec.y -= destRec.height * 0.5f;

    player = std::make_unique<Player>(
        startPos,
        texture,
        Rectangle{0.0f, 0.0f, (float)texture.width / 8.0f, (float)texture.height},
        destRec);

    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.offset = {(float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f};
    camera.target = player->position;
}

void World::Update(float delta)
{
    player->move(camera);
    camera.target = player->position;
}

void World::Presenter(float delta)
{
    int dir = (int)((player->angle + 22.5f) / 45.0f) % 8;
    BeginMode2D(camera);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            Vector2 gridPostion = IsoWorldToScreen(i, j);
            DrawTexturePro(
                background,
                {0.0f, 0.0f, (float)background.width, (float)background.height},
                {(float)(gridPostion.x), (float)(gridPostion.y), (float)background.width, (float)background.height},
                {0, 0},
                0.0f,
                WHITE);
        }
    }

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
            currentTexture = playerRunningTextureRight;
            break;
        case 1:
            currentTexture = playerRunningTextureRightDown;
            break;
        case 2:
            currentTexture = playerRunningTextureDown;
            break;
        case 3:
            currentTexture = playerRunningTextureLeftDown;
            break;
        case 4:
            currentTexture = playerRunningTextureLeft;
            break;
        case 5:
            currentTexture = playerRunningTextureLeftUp;
            break;
        case 6:
            currentTexture = playerRunningTextureUp;
            break;
        case 7:
            currentTexture = playerRunningTextureRightUp;
            break;
        default:
            currentTexture = playerRunningTextureDown;
            break;
        }

        DrawTexturePro(
            currentTexture,
            {(float)player->sourceRec.width * player->currentFrame, 0.0f, (float)player->sourceRec.width, (float)player->sourceRec.height},
            player->destRec,
            {0, 0},
            0.0f,
            WHITE);
    }

    EndMode2D();
}

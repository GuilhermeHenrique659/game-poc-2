// main.cpp
#include <raylib.h>
#include "common/Entity.h"   // seu header com Entity e Observer
#include "player/player.cpp" // seu Player (depois vamos separar isso tbm)
#include "enemy/Enemy.h"

#include <any>
#include <cstdio>

class PlayerMoved : public Observer
{
public:
    void notify(const std::any &data) override
    {
        Vector2 pos = std::any_cast<Vector2>(data);
        printf("Player moved -> X: %.1f | Y: %.1f\n", pos.x, pos.y);
    }
};

class EmenyFollow : public Observer
{
private:
    Enemy &enemy;

public:
    EmenyFollow(Enemy &enemy);
    ~EmenyFollow();

    void notify(const std::any &data) override
    {
        Vector2 post = std::any_cast<Vector2>(data);

        if (post.x < enemy.position.x + 400 && post.y < enemy.position.y + 400)
        {
            enemy.follow(post);
        }
    }
};

EmenyFollow::EmenyFollow(Enemy &enemy) : enemy(enemy)
{
}

EmenyFollow::~EmenyFollow() = default;

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    Vector2 position = {(float)screenWidth / 2, (float)screenHeight / 2};

    InitWindow(screenWidth, screenHeight, "Raylib - C++");
    SetTargetFPS(60);

    Player player = Player(position);
    Enemy enemy = Enemy({
        x : 100,
        y : 100,
    });

    player.subscribe("moved", std::make_unique<PlayerMoved>());
    player.subscribe("moved", std::make_unique<EmenyFollow>(EmenyFollow(enemy)));

    while (!WindowShouldClose())
    {
        player.move();

        if (CheckCollisionCircles(player.position, 50, enemy.position, 50))
            CloseWindow();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawCircleV(player.position, 50, MAROON);

        DrawCircleV(enemy.position, 50, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
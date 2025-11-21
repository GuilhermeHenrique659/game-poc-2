// main.cpp
#include <raylib.h>
#include "common/Subject.h"  // seu header com Entity e Observer
#include "player/player.cpp" // seu Player (depois vamos separar isso tbm)
#include "enemy/Enemy.h"
#include "common/ui/Button.h"
#include "scene/Scene.h"
#include "common/ui/Hover.h"
#include "scene/menu/Menu.h"

#include <any>
#include <cstdio>
#include <memory>

struct GameState
{
    bool isRunning;
    int currentScene;
};

class OnClick : public Observer
{
private:
    GameState *gameState;

public:
    OnClick(GameState *gameState) : gameState(gameState) {};
    ~OnClick() = default;
    void notify(const std::any &data) override
    {
        Button *button = std::any_cast<Button *>(data);

        TraceLog(LOG_INFO, "Button %s clicked!", button->label);

        gameState->isRunning = false;
    }
};

/* class OnHover : public Observer
{
private:

public:
    OnHover() = default;
    ~OnHover() = default;
    void notify(const std::any &data) override
    {
        Button *button = std::any_cast<Button *>(data);

        button->setColor(GRAY);
    }
}; */

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    Vector2 position = {(float)screenWidth / 2, (float)screenHeight / 2};

    GameState gameState = {
        .isRunning = true,
        .currentScene = 0,
    };

    InitWindow(screenWidth, screenHeight, "Raylib - C++");
    SetTargetFPS(60);

    Button button("Click Me", 350, 200, 100, 50);
    button.subscribe("OnHover", std::make_unique<OnHover>());

    std::unique_ptr<Scene> currentScene = std::make_unique<Menu>();

    currentScene->Setup();

    while (!WindowShouldClose())
    {
        currentScene->Update(GetFrameTime());

        BeginDrawing();
        ClearBackground(RAYWHITE);

        currentScene->Presenter(GetFrameTime());

        EndDrawing();

        if (gameState.isRunning == false)
        {
            break;
        }
    }

    CloseWindow();
    return 0;
}
// main.cpp
#include <raylib.h>
#include "common/Subject.h" // seu header com Entity e Observer
#include "enemy/Enemy.h"
#include "common/ui/Button.h"
#include "common/Scene.h"
#include "common/GameState.h"
#include "common/ui/Hover.h"
#include "scene/menu/Menu.h"
#include "scene/world/World.h"
#include <unordered_map>

#include <any>
#include <cstdio>
#include <memory>

int main()
{
    const int screenWidth = 1270;
    const int screenHeight = 720;
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;

    GameState gameState = {
        .isRunning = true,
        .currentScene = "menu",
    };

    InitWindow(screenWidth, screenHeight, "Raylib - C++");
    SetTargetFPS(60);

    Texture2D runDownTexture = LoadTexture("resources/sprites/down.png");
    Texture2D runUpTexture = LoadTexture("resources/sprites/up.png");
    Texture2D runLeftTexture = LoadTexture("resources/sprites/left.png");
    Texture2D runRightTexture = LoadTexture("resources/sprites/right.png");
    Texture2D runLeftUpTexture = LoadTexture("resources/sprites/left_up.png");
    Texture2D runRightUpTexture = LoadTexture("resources/sprites/right_up.png");
    Texture2D runLeftDownTexture = LoadTexture("resources/sprites/left_down.png");
    Texture2D runRightDownTexture = LoadTexture("resources/sprites/right_down.png");

    TraceLog(LOG_INFO, "runDownTexture id: %d", runDownTexture.id);

    scenes["menu"] = std::make_unique<Menu>(gameState);
    scenes["world"] = std::make_unique<World>(gameState, LoadTexture("resources/sprites/floor.png"), runDownTexture, runUpTexture, runLeftTexture, runRightTexture, runLeftUpTexture, runLeftDownTexture, runRightUpTexture, runRightDownTexture);

    std::unique_ptr<Scene> *currentScene = &scenes["menu"];

    for (auto &&scene : scenes)
    {
        scene.second->Setup();
    }

    while (!WindowShouldClose())
    {
        if (gameState.currentScene == "world")
        {
            currentScene = &scenes["world"];
        }

        currentScene->get()->Update(GetFrameTime());

        BeginDrawing();
        ClearBackground(RAYWHITE);

        currentScene->get()->Presenter(GetFrameTime());

        DrawFPS(10, 10);
        EndDrawing();

        if (gameState.isRunning == false)
        {
            TraceLog(LOG_INFO, "Exiting game loop.");
            break;
        }
    }

    delete currentScene;

    CloseWindow();
    return 0;
}
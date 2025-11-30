// main.cpp
#include <raylib_win.h>
#include "network/Network.h"

#include "common/Subject.h" // seu header com Entity e Observer
#include "enemy/Enemy.h"
#include "common/ui/Button.h"
#include "common/Scene.h"
#include "common/GameState.h"
#include "common/ui/Hover.h"
#include "scene/menu/Menu.h"
#include "scene/world/World.h"
#include <unordered_map>
#include "common/ResourceManager.h"

#include <any>
#include <cstdio>
#include <memory>

int main()
{
    const int screenWidth = 1270;
    const int screenHeight = 720;

    Network *network = new Network();
    EntityManager *entityManager = new EntityManager(network);

    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;

    GameState gameState = {
        .isRunning = true,
        .currentScene = "menu",
    };

    InitWindow(screenWidth, screenHeight, "Raylib - C++");
    SetTargetFPS(60);

    ResourceManager &resourceManager = ResourceManager::Get();

    resourceManager.RegisterTexture("run_down", "resources/sprites/down.png");
    resourceManager.RegisterTexture("run_up", "resources/sprites/up.png");
    resourceManager.RegisterTexture("run_left", "resources/sprites/left.png");
    resourceManager.RegisterTexture("run_right", "resources/sprites/right.png");
    resourceManager.RegisterTexture("run_left_up", "resources/sprites/left_up.png");
    resourceManager.RegisterTexture("run_right_up", "resources/sprites/right_up.png");
    resourceManager.RegisterTexture("run_left_down", "resources/sprites/left_down.png");
    resourceManager.RegisterTexture("run_right_down", "resources/sprites/right_down.png");
    resourceManager.RegisterTexture("idle_sheet", "resources/sprites/idle_sheet.png");

    resourceManager.RegisterTexture("floor", "resources/sprites/floor.png");

    scenes["menu"] = std::make_unique<Menu>(gameState);
    scenes["world"] = std::make_unique<World>(gameState, entityManager, network);

    std::unique_ptr<Scene> *currentScene = &scenes["menu"];

    for (auto &&scene : scenes)
    {
        scene.second->Setup();
    }

    while (!WindowShouldClose())
    {
        network->Update();
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

    CloseWindow();

    return 0;
}
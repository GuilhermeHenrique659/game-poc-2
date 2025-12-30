// main.cpp
#include <raylib_win.h>
#include "network/Network.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include "common/Subject.h"
#include "common/ui/Button.h"
#include "common/Scene.h"
#include "common/GameState.h"
#include "common/ui/Hover.h"
#include "scene/menu/Menu.h"
#include "scene/game/Game.h"
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

    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;

    GameState gameState = {
        .isRunning = true,
        .currentScene = "menu",
    };

    InitWindow(screenWidth, screenHeight, "Raylib - C++");
    SetTargetFPS(60);

    ResourceManager &resourceManager = ResourceManager::Get();

    resourceManager.AddMap("resources/map/map.tmx");

    // RUN
    resourceManager.RegisterTexture("run_down", "resources/sprites/char/run/Run_Down.png");
    resourceManager.RegisterTexture("run_up", "resources/sprites/char/run/Run_Up.png");
    resourceManager.RegisterTexture("run_left", "resources/sprites/char/run/Run_Left.png");
    resourceManager.RegisterTexture("run_right", "resources/sprites/char/run/Run_Right.png");
    resourceManager.RegisterTexture("run_up_left", "resources/sprites/char/run/Run_Left_Up.png");
    resourceManager.RegisterTexture("run_up_right", "resources/sprites/char/run/Run_Right_Up.png");
    resourceManager.RegisterTexture("run_down_left", "resources/sprites/char/run/Run_Down_Left.png");
    resourceManager.RegisterTexture("run_down_right", "resources/sprites/char/run/Run_Down_Right.png");

    // IDLE STAND
    resourceManager.RegisterTexture("idle_stand_up", "resources/sprites/char/idle_stand/Idle_Stand_Up.png");
    resourceManager.RegisterTexture("idle_stand_up_right", "resources/sprites/char/idle_stand/Idle_Stand_Up_Right.png");
    resourceManager.RegisterTexture("idle_stand_right", "resources/sprites/char/idle_stand/Idle_Stand_Right.png");
    resourceManager.RegisterTexture("idle_stand_down_right", "resources/sprites/char/idle_stand/Idle_Stand_Down_Right.png");
    resourceManager.RegisterTexture("idle_stand_down", "resources/sprites/char/idle_stand/Idle_Stand_Down.png");
    resourceManager.RegisterTexture("idle_stand_down_left", "resources/sprites/char/idle_stand/Idle_Stand_Down_Left.png");
    resourceManager.RegisterTexture("idle_stand_left", "resources/sprites/char/idle_stand/Idle_Stand_Left.png");
    resourceManager.RegisterTexture("idle_stand_up_left", "resources/sprites/char/idle_stand/Idle_Stand_Up_Left.png");

    // ATTACK
    resourceManager.RegisterTexture("attack_down", "resources/sprites/char/attack/Attack_01_Down.png");
    resourceManager.RegisterTexture("attack_up", "resources/sprites/char/attack/Attack_01_Up.png");
    resourceManager.RegisterTexture("attack_left", "resources/sprites/char/attack/Attack_01_Left.png");
    resourceManager.RegisterTexture("attack_right", "resources/sprites/char/attack/Attack_01_Right.png");
    resourceManager.RegisterTexture("attack_up_left", "resources/sprites/char/attack/Attack_01_Up_Left.png");
    resourceManager.RegisterTexture("attack_up_right", "resources/sprites/char/attack/Attack_01_Up_Right.png");
    resourceManager.RegisterTexture("attack_down_left", "resources/sprites/char/attack/Attack_01_Down_Left.png");
    resourceManager.RegisterTexture("attack_down_right", "resources/sprites/char/attack/Attack_01_Down_Right.png");

    resourceManager.RegisterTexture("floor", "resources/sprites/floor.png");
    resourceManager.RegisterTexture("block", "resources/sprites/block_E.png");
    resourceManager.RegisterTexture("ground", "resources/sprites/ground.png");
    resourceManager.RegisterTexture("wall_1", "resources/sprites/wall_1.png");
    resourceManager.RegisterTexture("wall_2", "resources/sprites/wall_2.png");

    scenes["menu"] = std::make_unique<Menu>(gameState);
    scenes["game"] = std::make_unique<Game>(gameState);

    std::unique_ptr<Scene> *currentScene = &scenes["menu"];

    for (auto &&scene : scenes)
    {
        scene.second->Setup();
    }

    while (!WindowShouldClose())
    {
        network->Update();
        if (gameState.currentScene == "game")
        {
            currentScene = &scenes["game"];
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
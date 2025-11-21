// main.cpp
#include <raylib.h>
#include "common/Subject.h"  // seu header com Entity e Observer
#include "player/player.cpp" // seu Player (depois vamos separar isso tbm)
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
    const int screenWidth = 800;
    const int screenHeight = 450;
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;

    GameState gameState = {
        .isRunning = true,
        .currentScene = "menu",
    };

    InitWindow(screenWidth, screenHeight, "Raylib - C++");
    SetTargetFPS(60);

    scenes["menu"] = std::make_unique<Menu>(gameState);
    scenes["world"] = std::make_unique<World>(gameState);

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
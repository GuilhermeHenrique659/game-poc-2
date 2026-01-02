#include "ButtonEvent.h"
#include <raylib.h>

void StartGameObserver::notify(const std::any &data)
{
    TraceLog(LOG_INFO, "Start game!");

    gameState.currentScene = "game";
}

void QuitGameObserver::notify(const std::any &data)
{
    TraceLog(LOG_INFO, "Quit game!");

    gameState.isRunning = false;
}

void HostGame::notify(const std::any &data)
{
    TraceLog(LOG_INFO, "Start as host");
    network->InitAsServer();

    gameState.currentScene = "game";
}

void JoinGame::notify(const std::any &data)
{
    TraceLog(LOG_INFO, "Try connect");
    if (network->InitAsClient())
    {
        TraceLog(LOG_INFO, "Connect with success");

        gameState.currentScene = "game";
    };
}

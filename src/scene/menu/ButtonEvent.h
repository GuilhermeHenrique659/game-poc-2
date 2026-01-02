#pragma once

#include <memory.h>

#include "../../common/scene.h"
#include "../../common/Subject.h"
#include "../../network/Network.h"

class QuitGameObserver : public Observer
{
private:
    GameState &gameState;

public:
    QuitGameObserver(GameState &gameState) : gameState(gameState) {};
    ~QuitGameObserver() = default;

    void notify(const std::any &data) override;
};

class StartGameObserver : public Observer
{
private:
    GameState &gameState;

public:
    StartGameObserver(GameState &gameState) : gameState(gameState) {};
    ~StartGameObserver() = default;

    void notify(const std::any &data) override;
};

class HostGame : public Observer
{
private:
    GameState &gameState;
    std::shared_ptr<Network> network;

public:
    HostGame(GameState &gameState, std::shared_ptr<Network> network) : gameState(gameState), network(network) {};
    ~HostGame() = default;

    void notify(const std::any &data) override;
};

class JoinGame : public Observer
{
private:
    GameState &gameState;
    std::shared_ptr<Network> network;

public:
    JoinGame(GameState &gameState, std::shared_ptr<Network> network) : gameState(gameState), network(network) {};
    ~JoinGame() = default;

    void notify(const std::any &data) override;
};

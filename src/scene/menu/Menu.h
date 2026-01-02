#pragma once
#include <vector>

#include "../../common/Scene.h"
#include "../../common/ui/Button.h"
#include "../../network/Network.h"

class Menu : public Scene
{
private:
    std::vector<std::unique_ptr<Button>> buttons;
    std::shared_ptr<Network> network;

public:
    Menu(GameState &gameState, std::shared_ptr<Network> network) : Scene(gameState), network(network) {};
    ~Menu() = default;

    void Setup();
    void Presenter(float delta);
    void Update(float delta);
};
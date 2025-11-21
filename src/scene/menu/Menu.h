#pragma once
#include "../../common/Scene.h"
#include "../../common/ui/Button.h"
#include <vector>

class Menu : public Scene
{
private:
    std::vector<std::unique_ptr<Button>> buttons;

public:
    Menu(GameState &gameState) : Scene(gameState) {};
    ~Menu() = default;

    void Setup();
    void Presenter(float delta);
    void Update(float delta);
};
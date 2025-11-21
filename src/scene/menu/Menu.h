#pragma once
#include "../Scene.h"
#include "../../common/ui/Button.h" // ou o caminho correto
#include <vector>

class Menu : public Scene
{
private:
    std::vector<std::unique_ptr<Button>> buttons;

public:
    Menu() = default;
    ~Menu() = default;

    void Setup();
    void Presenter(float delta);
    void Update(float delta);
};
#include "raylib.h"
#include "Menu.h"
#include "../../common/ui/Hover.h"

void Menu::Setup()
{
    std::unique_ptr<Button> statButton = std::make_unique<Button>("Start", 350, 250, 100, 50);
    std::unique_ptr<Button> quitButton = std::make_unique<Button>("Exit", 350, 320, 100, 50);

    quitButton->subscribe("OnHover", std::make_unique<OnHover>());
    statButton->subscribe("OnHover", std::make_unique<OnHover>());

    buttons.emplace_back(std::move(statButton));
    buttons.emplace_back(std::move(quitButton));
}

void Menu::Presenter(float delta)
{
    DrawText("Menu Scene", 350, 200, 20, DARKGRAY);

    for (auto &button : buttons)
    {
        button->Update();
    }
}

void Menu::Update(float delta)
{
    for (auto &button : buttons)
    {
        button->Draw();
    }
}
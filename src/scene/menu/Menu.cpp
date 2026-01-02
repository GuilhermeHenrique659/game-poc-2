#include "raylib.h"
#include "Menu.h"

#include "../../common/ui/Hover.h"
#include "ButtonEvent.h"

void Menu::Setup()
{
    TraceLog(LOG_INFO, "Scene: %s", gameState.currentScene);
    std::unique_ptr<Button> start_button = std::make_unique<Button>("Start", 350, 250, 100, 50);
    std::unique_ptr<Button> host_button = std::make_unique<Button>("Host", 350, 320, 100, 50);
    std::unique_ptr<Button> join_button = std::make_unique<Button>("Join", 350, 390, 100, 50);
    std::unique_ptr<Button> quit_button = std::make_unique<Button>("Exit", 350, 460, 100, 50);

    start_button->subscribe("OnClick", std::make_unique<StartGameObserver>(gameState));
    host_button->subscribe("OnClick", std::make_unique<HostGame>(gameState, network));
    join_button->subscribe("OnClick", std::make_unique<JoinGame>(gameState, network));
    quit_button->subscribe("OnClick", std::make_unique<QuitGameObserver>(gameState));

    start_button->subscribe("OnHover", std::make_unique<OnHover>());
    host_button->subscribe("OnHover", std::make_unique<OnHover>());
    join_button->subscribe("OnHover", std::make_unique<OnHover>());
    quit_button->subscribe("OnHover", std::make_unique<OnHover>());

    buttons.emplace_back(std::move(start_button));
    buttons.emplace_back(std::move(host_button));
    buttons.emplace_back(std::move(join_button));
    buttons.emplace_back(std::move(quit_button));
}

void Menu::Presenter(float delta)
{
    DrawText("Menu Scene", 350, 200, 20, DARKGRAY);
    for (auto &button : buttons)
    {
        button->Draw();
    }
}

void Menu::Update(float delta)
{

    for (auto &button : buttons)
    {
        button->Update();
    }
}
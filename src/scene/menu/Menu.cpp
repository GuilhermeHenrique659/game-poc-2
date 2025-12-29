#include "raylib.h"
#include "Menu.h"
#include "../../common/ui/Hover.h"

class QuitGameObserver : public Observer
{
private:
    GameState &gameState;

public:
    QuitGameObserver(GameState &gameState) : gameState(gameState) {};
    ~QuitGameObserver() = default;
    void notify(const std::any &data) override
    {
        TraceLog(LOG_INFO, "Quit game!");

        gameState.isRunning = false;
    }
};

class StartGameObserver : public Observer
{
private:
    GameState &gameState;

public:
    StartGameObserver(GameState &gameState) : gameState(gameState) {};
    ~StartGameObserver() = default;
    void notify(const std::any &data) override
    {
        TraceLog(LOG_INFO, "Start game!");

        gameState.currentScene = "game";
    }
};

void Menu::Setup()
{
    TraceLog(LOG_INFO, "Scene: %s", gameState.currentScene);
    std::unique_ptr<Button> startButton = std::make_unique<Button>("Start", 350, 250, 100, 50);
    std::unique_ptr<Button> quitButton = std::make_unique<Button>("Exit", 350, 320, 100, 50);

    quitButton->subscribe("OnClick", std::make_unique<QuitGameObserver>(gameState));
    startButton->subscribe("OnClick", std::make_unique<StartGameObserver>(gameState));
    quitButton->subscribe("OnHover", std::make_unique<OnHover>());
    startButton->subscribe("OnHover", std::make_unique<OnHover>());

    buttons.emplace_back(std::move(startButton));
    buttons.emplace_back(std::move(quitButton));
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
#include "Game.h"
#include "../../entity/player/Player.h"
#include "../../view/player/PlayerView.h"
#include "../../command/PlayerMoveCommand.h"

void Game::Setup()
{
    entity_manager = std::make_shared<EntityManager>();
    view_manager = std::make_unique<ViewManager>();

    local_player_id = entity_manager->GetEntityId();
    auto player = std::make_shared<Player>(local_player_id, "Player", std::make_unique<EntityPosition>(Vector2{100.0f, 100.0f}, Direction::DOWN, Rectangle{100.0f, 100.0f, 320.0f, 320.0f}, Rectangle{100.0f, 100.0f, 320.0f, 320.0f}, 200.0f));

    entity_manager->AddEntity(player);
    view_manager->CreateView(player);
}

void Game::Update(float delta)
{
    auto move_command = std::make_unique<PlayerMoveCommand>(entity_manager);
    move_command->Execute(local_player_id);
}

void Game::Presenter(float delta)
{
    for (auto &[id, entity] : entity_manager->GetEntities())
    {
        auto view = view_manager->GetView(id);
        if (view)
        {
            if (entity->label == "Player")
            {
                PlayerView::Draw(entity, view);
            }

            auto texture = view->GetTexture();
            auto source_rect = view->GetSourceRectangle();

            DrawTexturePro(
                texture,
                source_rect,
                entity->GetDestReactangle(),
                {0, 0},
                0,
                Fade(WHITE, 0.8f));
        }
    }
}

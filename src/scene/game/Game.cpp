#include "Game.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#include "../../common/util/VectorUtil.h"
#include "../../entity/player/Player.h"
#include "../../view/player/PlayerView.h"
#include "../../command/PlayerMoveCommand.h"
#include "../../command/PlayerAttackCommand.h"
#include "../../common/ResourceManager.h"

void Game::Setup()
{
    local_player_id = entity_manager->GetEntityId();
    auto player_position = std::make_unique<EntityPosition>(Vector2{100.0f, 100.0f}, Direction::DOWN, Rectangle{100.0f, 100.0f, 320.0f, 320.0f}, Rectangle{100.0f, 100.0f, 320.0f, 320.0f}, 6.0f);
    auto player = std::make_shared<Player>(local_player_id, "Player", std::move(player_position));

    entity_manager->AddEntity(player);
    view_manager->CreateView(player);

    world_camera = CameraComponent::create(player->GetPosition(), player->GetDestReactangle());

    map->Init();
}

void Game::Update(float delta)
{
    auto local_player_inputs = input_manager->CaptureInput(local_player_id);

    auto move_command = std::make_unique<PlayerMoveCommand>(entity_manager);
    move_command->Execute(local_player_id, local_player_inputs);

    auto attack_command = std::make_unique<PlayerAttackCommand>(entity_manager);
    attack_command->Execute(local_player_id, local_player_inputs);

    world_camera->update(
        entity_manager->GetEntity(local_player_id).value()->GetPosition(),
        entity_manager->GetEntity(local_player_id).value()->GetDestReactangle());
}

void Game::Presenter(float delta)
{
    BeginMode2D(world_camera->getCamera());

    map->Draw("floor");
    map->Draw("block");

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

    EndMode2D();
}

#include "Game.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <algorithm>

#include "../../common/util/VectorUtil.h"
#include "../../entity/player/Player.h"
#include "../../view/player/PlayerView.h"
#include "../../command/PlayerMoveCommand.h"
#include "../../command/PlayerAttackCommand.h"
#include "../../common/ResourceManager.h"

void Game::Setup()
{
    local_player_id = entity_manager->GetEntityId();
    auto player_position = std::make_unique<EntityPosition>(Vector2{400.0f, 400.0f}, Direction::DOWN, Rectangle{400.0f, 400.0f, 320.0f, 320.0f}, Rectangle{}, 6.0f);
    auto player = std::make_shared<Player>(local_player_id, "Player", std::move(player_position));

    entity_manager->AddEntity(player);
    view_manager->CreateView(player);

    world_camera = CameraComponent::create(player->GetPosition(), player->GetDestReactangle());

    map->Init();
}

bool sortSprite(const Sprite &a, const Sprite &b)
{
    return a.position.y < b.position.y;
}

void Game::Update(float delta)
{
    auto local_player_inputs = input_manager->CaptureInput(local_player_id);

    auto move_command = std::make_unique<PlayerMoveCommand>(entity_manager);
    move_command->Execute(local_player_id, local_player_inputs, map->GetCollisionLines());

    auto attack_command = std::make_unique<PlayerAttackCommand>(entity_manager);
    attack_command->Execute(local_player_id, local_player_inputs);

    world_camera->update(
        entity_manager->GetEntity(local_player_id).value()->GetPosition(),
        entity_manager->GetEntity(local_player_id).value()->GetDestReactangle());
}

void Game::Presenter(float delta)
{
    BeginMode2D(world_camera->getCamera());

    map->Draw("ground");

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        TraceLog(LOG_INFO, TextFormat("X: %f, Y: %f", GetMousePosition().x, GetMousePosition().y));
    }

    std::vector<Sprite> drawables = map->GetSpritesByLayer("obj");

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

            Vector2 entity_feet = entity->GetEntityFeet();
            float collisionRadius = (entity->GetDestReactangle().width * 0.1f);

            DrawCircleLinesV(entity_feet, collisionRadius, RED);

            drawables.push_back({texture,
                                 entity_feet,
                                 source_rect,
                                 entity->GetDestReactangle(),
                                 "Entity"});
        }
    }

    std::sort(drawables.begin(), drawables.end(), sortSprite);

    for (auto &sprite : drawables)
    {
        if (sprite.type == "Entity")
        {
            DrawRectangleLinesEx(sprite.dest_rectangle, 2, GREEN);

            DrawText(
                TextFormat("x: %.0f y: %.0f", sprite.dest_rectangle.x, sprite.dest_rectangle.y),
                sprite.dest_rectangle.x,
                sprite.dest_rectangle.y - 12,
                10,
                GREEN);
        }

        DrawTexturePro(
            sprite.texture,
            sprite.source_rectangle,
            sprite.dest_rectangle,
            {0, 0},
            0,
            WHITE);
    }

    for (auto &i : map->GetCollisionLines())
    {
        DrawLineV(i.start, i.end, RED);
    }

    EndMode2D();
}

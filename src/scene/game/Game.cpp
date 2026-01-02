#include "Game.h"
#include "GameEvents.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <algorithm>

#include "../../network/Events.h"
#include "../../network/PacketSerialization.hpp"
#include "../../common/util/VectorUtil.h"
#include "../../entity/player/Player.h"
#include "../../view/player/PlayerView.h"
#include "../../command/PlayerMoveCommand.h"
#include "../../command/PlayerAttackCommand.h"
#include "../../common/ResourceManager.h"

void Game::Setup()
{
    local_player_id = entity_manager->GetEntityId();
    auto player = Player::Create(local_player_id, {400.0f, 400.0f}, Direction::DOWN);

    entity_manager->AddEntity(player);
    view_manager->CreateView(player);

    world_camera = CameraComponent::create(player->GetPosition(), player->GetDestReactangle());

    if (network->isServer && network->isConnect)
    {
        network->addListner(EVENT_DEFINITIONS[EventName::CONNECTION].name, std::make_unique<OnConnected>(entity_manager, network, view_manager));
        network->addListner(EVENT_DEFINITIONS[EventName::REMOTE_INPUT].name, std::make_unique<OnInputReceive>(input_manager));
    }

    if (!network->isServer && network->isConnect)
    {
        network->addListner(EVENT_DEFINITIONS[EventName::ID_ASSIGNEED].name, std::make_unique<OnIdAssigneed>(entity_manager, view_manager, local_player_id));
        network->addListner(EVENT_DEFINITIONS[EventName::WORLD_SNAPSHOT].name, std::make_unique<OnSnapshotReceive>(entity_manager, view_manager));

        ConnectClient();
    }

    map->Init();
}

bool sortSprite(const Sprite &a, const Sprite &b)
{
    return a.position.y < b.position.y;
}

void Game::Update(float delta)
{
    auto local_player_inputs = input_manager->CaptureInput(local_player_id);

    if (network->isServer || !network->isConnect)
    {
        auto inputs = input_manager->GetInputs();

        for (auto &[player_id, player_input] : inputs)
        {
            auto move_command = std::make_unique<PlayerMoveCommand>(entity_manager);
            move_command->Execute(player_id, player_input, map->GetCollisionLines());

            auto attack_command = std::make_unique<PlayerAttackCommand>(entity_manager);
            attack_command->Execute(player_id, player_input);
        }

        if (network->isServer)
        {
            BroadcastEntitiesSnapshot();
        }
    }

    if (network->isConnect)
    {
        RemoteInput remote_input = {
            .player_id = local_player_id,
            .inputs = local_player_inputs,
        };

        auto pkt = serializePacket<RemoteInput>(EVENT_DEFINITIONS[EventName::REMOTE_INPUT].id, remote_input);

        network->Send(&pkt, sizeof(RemoteInput));
    }

    if (entity_manager->GetEntity(local_player_id).has_value())
        world_camera->update(
            entity_manager->GetEntity(local_player_id).value()->GetPosition(),
            entity_manager->GetEntity(local_player_id).value()->GetDestReactangle());
}

void Game::ConnectClient()
{
    auto entity = entity_manager->GetEntity(local_player_id);

    if (entity.has_value())
    {
        auto player = std::dynamic_pointer_cast<Player>(entity.value());

        RemotePlayerDTO player_dto = {
            .player_id = local_player_id,
            .position = player->GetPosition(),
            .direction = player->GetEntityDirection(),
            .state = (int)player->GetState(),
        };
        RemotePacket pkg = serializePacket(EVENT_DEFINITIONS[EventName::CONNECTION].id, player_dto);

        network->Send(&pkg, sizeof(RemotePlayerDTO));
    }
}

void Game::BroadcastEntitiesSnapshot()
{
    for (auto &[entity_id, entity] : entity_manager->GetEntities())
    {
        auto player = std::dynamic_pointer_cast<Player>(entity);

        RemotePlayerDTO player_dto = {
            .player_id = entity_id,
            .position = player->GetPosition(),
            .direction = player->GetEntityDirection(),
            .state = (int)player->GetState(),
        };

        auto pkg = serializePacket<RemotePlayerDTO>(EVENT_DEFINITIONS[EventName::WORLD_SNAPSHOT].id, player_dto);

        network->Send(&pkg, sizeof(RemotePlayerDTO));
    }
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

    DrawText(TextFormat("ID: %d, is %s", local_player_id, network->isServer ? "host" : "client"), 100, 10, 20, BLUE);
}

#include "Game.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <algorithm>

#include "../../network/Events.h"
#include "../../common/util/VectorUtil.h"
#include "../../entity/player/Player.h"
#include "../../view/player/PlayerView.h"
#include "../../command/PlayerMoveCommand.h"
#include "../../command/PlayerAttackCommand.h"
#include "../../common/ResourceManager.h"

struct PlayerSnapshot
{
    uint32_t player_id;
    Vector2 position;
    Direction direction;
    int state;
};

struct RemoveInput
{
    uint32_t player_id;
    Inputs inputs;
};

struct NewIdAssignee
{
    uint32_t player_id;
    uint32_t new_player_id;
};

class OnConnected : public Observer
{
private:
    std::shared_ptr<EntityManager> entity_manager;
    std::shared_ptr<Network> network;

public:
    OnConnected(std::shared_ptr<EntityManager> entity_manager, std::shared_ptr<Network> network) : entity_manager(entity_manager), network(network) {};
    ~OnConnected() = default;

    void notify(const std::any &data)
    {
        RemotePacket pkg = std::any_cast<RemotePacket>(data);
        PlayerSnapshot pm{};
        memcpy(&pm, pkg.data, sizeof(PlayerSnapshot));

        auto new_id = entity_manager->GetEntityId();
        auto player_position = std::make_unique<EntityPosition>(pm.position, pm.direction, Rectangle{pm.position.y, pm.position.y, 320.0f, 320.0f}, Rectangle{}, 6.0f);
        auto remote_player = std::make_shared<Player>(new_id, "Player", std::move(player_position));

        entity_manager->AddEntity(remote_player);

        NewIdAssignee snapshot = {
            .player_id = pm.player_id,
            .new_player_id = new_id};

        const size_t dataSize = sizeof(PlayerSnapshot);
        RemotePacket pkt;

        pkt.eventId = EVENT_DEFINITIONS[EventName::ID_ASSIGNEED].id;

        memcpy(&pkt.data, &snapshot, dataSize);

        network->Send(&pkt, dataSize);
    }
};

class OnIdAssigneed : public Observer
{
private:
    std::shared_ptr<EntityManager> entity_manager;
    uint32_t &local_id;

public:
    OnIdAssigneed(std::shared_ptr<EntityManager> entity_manager, uint32_t &local_id) : entity_manager(entity_manager), local_id(local_id) {};
    ~OnIdAssigneed() = default;

    void notify(const std::any &data)
    {
        RemotePacket pkg = std::any_cast<RemotePacket>(data);
        NewIdAssignee pm{};
        memcpy(&pm, pkg.data, sizeof(NewIdAssignee));

        auto entity = entity_manager->GetEntity(pm.player_id);
        entity.value()->id = pm.new_player_id;
        local_id = pm.new_player_id;
    }
};

class OnInputReceive : public Observer
{
private:
    std::shared_ptr<InputManager> input_manager;

public:
    OnInputReceive(std::shared_ptr<InputManager> input_manager) : input_manager(input_manager) {};
    ~OnInputReceive() = default;

    void notify(const std::any &data)
    {
        RemotePacket pkg = std::any_cast<RemotePacket>(data);
        RemoveInput pm{};
        memcpy(&pm, pkg.data, sizeof(RemoveInput));

        input_manager->ReceiveInput(pm.player_id, pm.inputs);
    }
};

class OnSnapshotReceive : public Observer
{
private:
    std::shared_ptr<EntityManager> entity_manager;

public:
    OnSnapshotReceive(std::shared_ptr<EntityManager> entity_manager) : entity_manager(entity_manager) {};
    ~OnSnapshotReceive() = default;

    void notify(const std::any &data)
    {
        RemotePacket pkg = std::any_cast<RemotePacket>(data);
        PlayerSnapshot pm{};
        memcpy(&pm, pkg.data, sizeof(PlayerSnapshot));

        auto option = entity_manager->GetEntity(pm.player_id);

        if (option.has_value())
        {
            auto player = std::dynamic_pointer_cast<Player>(option.value());

            player->UpdatePosition(pm.position);
            player->SetDirection(pm.direction);
            player->ChangeState(PlayerState{pm.state});
        }
    }
};

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
            for (auto &[entity_id, entity] : entity_manager->GetEntities())
            {
                auto player = std::dynamic_pointer_cast<Player>(entity);

                PlayerSnapshot snapshot = {
                    .player_id = entity_id,
                    .position = player->GetPosition(),
                    .direction = player->GetEntityDirection(),
                    .state = (int)player->GetState(),
                };
                const size_t dataSize = sizeof(PlayerSnapshot);
                RemotePacket pkt;

                pkt.eventId = EVENT_DEFINITIONS[EventName::WORLD_SNAPSHOT].id;

                memcpy(&pkt.data, &snapshot, dataSize);

                network->Send(&pkt, dataSize);
            }
        }
    }

    if (IsKeyPressed(KEY_H))
    {

        TraceLog(LOG_INFO, "Start as host");
        network->InitAsServer(); // tecla H = host

        network->addListner("connection", std::make_unique<OnConnected>(entity_manager, network));
    }
    if (IsKeyPressed(KEY_C))
    {
        TraceLog(LOG_INFO, "Try connect");
        network->InitAsClient(); // tecla C = conectar em 127.0.0.1

        network->addListner("id_assign", std::make_unique<OnIdAssigneed>(entity_manager, local_player_id));
        network->addListner("world_snapshot", std::make_unique<OnSnapshotReceive>(entity_manager));

        auto entity = entity_manager->GetEntity(local_player_id);
        auto player = std::dynamic_pointer_cast<Player>(entity.value());

        PlayerSnapshot snapshot = {
            .player_id = local_player_id,
            .position = player->GetPosition(),
            .direction = player->GetEntityDirection(),
            .state = (int)player->GetState(),
        };
        const size_t dataSize = sizeof(PlayerSnapshot);
        RemotePacket pkt;

        pkt.eventId = EVENT_DEFINITIONS[EventName::CONNECTION].id;

        memcpy(&pkt.data, &snapshot, dataSize);

        network->Send(&pkt, dataSize);
    }

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

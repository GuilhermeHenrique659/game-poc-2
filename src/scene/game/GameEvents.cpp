#include "GameEvents.h"

#include "../../entity/player/Player.h"
#include "../../network/Events.h"
#include "../../network/packetSerialization.hpp"

void OnConnected::notify(const std::any &data)
{
    auto player_data = deserializePayload<RemotePlayerDTO>(data);

    auto new_id = entity_manager->GetEntityId();
    auto remote_player = Player::Create(new_id, player_data.position, player_data.direction);

    entity_manager->AddEntity(remote_player);
    view_manager->CreateView(remote_player);

    TraceLog(LOG_INFO, "Player Connected: %d", new_id);

    NewIdAssignee payload = {
        .player_id = player_data.player_id,
        .new_player_id = new_id};

    auto pkg = serializePacket<NewIdAssignee>(EVENT_DEFINITIONS[EventName::ID_ASSIGNEED].id, payload);

    network->Send(&pkg, sizeof(NewIdAssignee));
}

void OnIdAssigneed::notify(const std::any &data)
{
    NewIdAssignee pm = deserializePayload<NewIdAssignee>(data);

    TraceLog(LOG_INFO, "Id Assigneed: %d", pm.new_player_id);

    if (pm.player_id != local_id)
        return;

    auto entity = entity_manager->GetEntity(pm.player_id);
    if (!entity.has_value())
        return;

    entity.value()->id = pm.new_player_id;
    local_id = pm.new_player_id;

    entity_manager->RemoveEntity(pm.player_id);

    entity_manager->AddEntity(entity.value());
    view_manager->CreateView(entity.value());
}

void OnInputReceive::notify(const std::any &data)
{
    RemoteInput pm = deserializePayload<RemoteInput>(data);

    input_manager->ReceiveInput(pm.player_id, pm.inputs);
}

void OnSnapshotReceive::notify(const std::any &data)
{
    RemotePlayerDTO remote_player_data = deserializePayload<RemotePlayerDTO>(data);

    auto option = entity_manager->GetEntity(remote_player_data.player_id);

    if (option.has_value())
    {
        auto player = std::dynamic_pointer_cast<Player>(option.value());

        player->UpdatePosition(remote_player_data.position);
        player->SetDirection(remote_player_data.direction);
        player->ChangeState(PlayerState{remote_player_data.state});
    }
    else
    {
        auto remote_player = Player::Create(remote_player_data.player_id, remote_player_data.position, remote_player_data.direction);

        entity_manager->AddEntity(remote_player);
        view_manager->CreateView(remote_player);
    }
}
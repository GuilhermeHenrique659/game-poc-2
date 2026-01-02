#pragma once

#include "../../common/Subject.h"
#include "../../common/input/InputManager.h"
#include "../../entity/EntityManager.h"
#include "../../view/ViewManager.h"
#include "../../network/Network.h"

struct RemotePlayerDTO
{
    uint32_t player_id;
    Vector2 position;
    Direction direction;
    int state;
};

struct RemoteInput
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
    std::shared_ptr<ViewManager> view_manager;

public:
    OnConnected(std::shared_ptr<EntityManager> entity_manager, std::shared_ptr<Network> network, std::shared_ptr<ViewManager> view_manager) : entity_manager(entity_manager), network(network), view_manager(view_manager) {};
    ~OnConnected() = default;

    void notify(const std::any &data) override;
};

class OnIdAssigneed : public Observer
{
private:
    std::shared_ptr<EntityManager> entity_manager;
    std::shared_ptr<ViewManager> view_manager;
    uint32_t &local_id;

public:
    OnIdAssigneed(std::shared_ptr<EntityManager> entity_manager, std::shared_ptr<ViewManager> view_manager, uint32_t &local_id) : entity_manager(entity_manager), view_manager(view_manager), local_id(local_id) {};
    ~OnIdAssigneed() = default;

    void notify(const std::any &data) override;
};

class OnInputReceive : public Observer
{
private:
    std::shared_ptr<InputManager> input_manager;

public:
    OnInputReceive(std::shared_ptr<InputManager> input_manager) : input_manager(input_manager) {};
    ~OnInputReceive() = default;

    void notify(const std::any &data) override;
};

class OnSnapshotReceive : public Observer
{
private:
    std::shared_ptr<EntityManager> entity_manager;
    std::shared_ptr<ViewManager> view_manager;

public:
    OnSnapshotReceive(std::shared_ptr<EntityManager> entity_manager, std::shared_ptr<ViewManager> view_manager) : entity_manager(entity_manager), view_manager(view_manager) {};
    ~OnSnapshotReceive() = default;

    void notify(const std::any &data) override;
};
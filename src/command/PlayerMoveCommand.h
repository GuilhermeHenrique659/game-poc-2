#pragma once

#include <cstdint>
#include <memory>

#include "../entity/EntityManager.h"

class PlayerMoveCommand
{
private:
    std::shared_ptr<EntityManager> entity_manager;

public:
    PlayerMoveCommand(std::shared_ptr<EntityManager> entity_manager) : entity_manager(entity_manager) {}
    ~PlayerMoveCommand() = default;

    void Execute(uint32_t player_id);
};
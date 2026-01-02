#pragma once

#include <cstdint>
#include <memory>

#include "../entity/EntityManager.h"
#include "../common/input/Inputs.h"
#include "../common/CollisionLines.h"

class PlayerMoveCommand
{
private:
    std::shared_ptr<EntityManager> entity_manager;

public:
    PlayerMoveCommand(std::shared_ptr<EntityManager> entity_manager) : entity_manager(entity_manager) {}
    ~PlayerMoveCommand() = default;

    void Execute(uint32_t player_id, const Inputs &input, std::vector<CollisionLines> collision_lines);
};
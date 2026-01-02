#pragma once

#include <cstdint>
#include <memory>

#include "../entity/EntityManager.h"
#include "../common/input/Inputs.h"

class PlayerAttackCommand
{
private:
    std::shared_ptr<EntityManager> entity_manager;

public:
    PlayerAttackCommand(std::shared_ptr<EntityManager> entity_manager) : entity_manager(entity_manager) {}
    ~PlayerAttackCommand() = default;

    void Execute(uint32_t player_id, const Inputs &input);
};
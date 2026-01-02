#pragma once
#include <raylib.h>
#include <unordered_map>
#include <cstdint>
#include "Inputs.h"

class InputManager
{
private:
    std::unordered_map<uint32_t, Inputs> all_inputs;

public:
    InputManager(/* args */) = default;
    ~InputManager() = default;

    void ReceiveInput(uint32_t player_id, Inputs input);
    Inputs CaptureInput(uint32_t player_id);
    std::unordered_map<uint32_t, Inputs> GetInputs();
    void ClearInputs();
};
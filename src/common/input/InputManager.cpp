#include "InputManager.h"

void InputManager::ReceiveInput(uint32_t player_id, Inputs input)
{
    all_inputs[player_id] = input;
}

Inputs InputManager::CaptureInput(uint32_t player_id)
{
    Inputs input_states;

    input_states.move_up = IsKeyDown(KEY_UP);
    input_states.move_down = IsKeyDown(KEY_DOWN);
    input_states.move_left = IsKeyDown(KEY_LEFT);
    input_states.move_right = IsKeyDown(KEY_RIGHT);

    input_states.attack = IsKeyDown(KEY_SPACE);

    return input_states;
}

std::unordered_map<uint32_t, Inputs> InputManager::GetInputs(uint32_t player_id)
{
    return all_inputs;
}

void InputManager::ClearInputs()
{
    all_inputs.clear();
}
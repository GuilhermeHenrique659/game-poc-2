#pragma once;

#include "../../common/Subject.h"
#include <cstdint>
#include <string>

class Entity : public Subject
{
private:
public:
    uint32_t id;
    std::string label;

    Entity(std::string label, uint32_t id) : id(id), label(label) {};
    ~Entity() = default;
};

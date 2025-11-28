#pragma once
#include <cstdint>

const int ID_MEMORY_SIZE = 4;

struct RemotePacket
{
    int32_t eventId;
    uint8_t data[1024];
};

#pragma once
#include <cstdint>

struct RemotePacket
{
    int32_t eventId;    // PRIMEIRO!
    uint8_t data[1024]; // dados depois
};

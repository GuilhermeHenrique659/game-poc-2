#pragma once

#include "Events.h"
#include "Network.h"

#include <cstring>
#include <type_traits>

template <typename Payload>
RemotePacket serializePacket(int eventId, const Payload &payload)
{
    RemotePacket pkg{};
    pkg.eventId = eventId;

    constexpr size_t data_size = sizeof(Payload);
    static_assert(data_size <= sizeof(pkg.data),
                  "Payload maior que o buffer de data no RemotePacket!");

    std::memcpy(pkg.data, &payload, data_size);

    return pkg;
}

template <typename Payload>
Payload deserializePayload(const std::any &data)
{
    auto pkg = std::any_cast<RemotePacket>(data);

    constexpr size_t expected_size = sizeof(Payload);
    static_assert(expected_size <= sizeof(pkg.data),
                  "Buffer de data muito pequeno para este payload!");

    Payload payload{};
    std::memcpy(&payload, pkg.data, expected_size);

    return payload;
}
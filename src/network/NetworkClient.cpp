#include "NetworkClient.h"
#include "Events.h"

void NetworkClient::Handle(int32_t eventId, RemotePacket packet)
{
    _publish(EVENT_DEFINITIONS[eventId].name, packet);
}
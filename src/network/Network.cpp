#include "Network.h"
#include "Package.h"
#include <iostream>

bool Network::InitAsServer(uint16_t port)
{
    if (enet_initialize() != 0)
        return false;

    address.host = ENET_HOST_ANY;
    address.port = port;

    host = enet_host_create(&address, 8, 2, 0, 0);
    if (!host)
        return false;

    isServer = true;

    std::cout << "Server criado na porta " << port << std::endl;
    return true;
}

bool Network::InitAsClient(const std::string &ip, uint16_t port)
{
    if (enet_initialize() != 0)
        return false;

    host = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!host)
        return false;

    ENetAddress addr;
    enet_address_set_host(&addr, ip.c_str());
    addr.port = port;

    ENetPeer *peer = enet_host_connect(host, &addr, 2, 0);
    if (!peer)
        return false;

    if (enet_host_service(host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        std::cout << "Conectado ao servidor!" << std::endl;
        isServer = false;
        return true;
    }

    enet_peer_reset(peer);
    return false;
}

void Network::Update()
{
    if (!host)
        return;

    while (enet_host_service(host, &event, 0) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
        {
            if (isServer)
            {
                if (networkClient)
                    networkClient->Handle(0, {});
            }
            break;
        }

        case ENET_EVENT_TYPE_RECEIVE:
        {
            const uint8_t *d = event.packet->data;
            size_t len = event.packet->dataLength;

            if (len >= ID_MEMORY_SIZE)
            {
                RemotePacket pkt{};

                pkt.eventId = *(int32_t *)d;

                size_t payload = len - ID_MEMORY_SIZE;
                if (payload > sizeof(pkt.data))
                    payload = sizeof(pkt.data);

                memcpy(pkt.data, d + ID_MEMORY_SIZE, payload);

                networkClient->Handle(pkt.eventId, pkt);

                if (isServer)
                {
                    ENetPacket *copy = enet_packet_create(d, len, 0);
                    enet_host_broadcast(host, 0, copy);
                }
            }

            enet_packet_destroy(event.packet);
            break;
        }

        case ENET_EVENT_TYPE_DISCONNECT:
        {
            printf("Cliente desconectado.\n");
            break;
        }
        }
    }
}

void Network::Send(RemotePacket *remotePacket, size_t dataSize)
{
    if (!host)
        return;

    const size_t totalSize = sizeof(int32_t) + dataSize;
    std::vector<uint8_t> buffer(totalSize);

    memcpy(buffer.data(), &remotePacket->eventId, sizeof(int32_t));
    memcpy(buffer.data() + 4, remotePacket->data, dataSize);

    ENetPacket *packet = enet_packet_create(
        buffer.data(),
        totalSize,
        0);

    enet_host_broadcast(host, 0, packet);
}

void Network::Shutdown()
{
    if (host)
    {
        enet_host_destroy(host);
        host = nullptr;
    }

    enet_deinitialize();
    isServer = false;
}

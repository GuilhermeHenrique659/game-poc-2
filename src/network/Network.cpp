#include "Network.h"
#include <iostream>

bool Network::InitAsServer(uint16_t port)
{
    if (enet_initialize() != 0)
        return false;

    address.host = ENET_HOST_ANY;
    address.port = port;

    host = enet_host_create(&address, 8, 2, 0, 0); // max 8 players
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

    // Espera conexão
    if (enet_host_service(host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
    {
        std::cout << "Conectado ao servidor!" << std::endl;
        isServer = false;
        return true;
    }

    enet_peer_reset(peer);
    return false;
}

void Network::Update(float delta)
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
                uint32_t id = nextPlayerId++;
                players[id].peer = event.peer;
                event.peer->data = (void *)(uintptr_t)id;

                // Envia ID ao cliente
                char packet[8];
                packet[0] = 0; // tipo: assign_id
                memcpy(packet + 1, &id, 4);
                enet_peer_send(event.peer, 0, enet_packet_create(packet, 5, ENET_PACKET_FLAG_RELIABLE));
            }
            break;
        }
        case ENET_EVENT_TYPE_RECEIVE:
        {
            // Agora aceita exatamente 15 bytes (ou mais, se quiser expandir no futuro)
            if (event.packet->dataLength >= 15)
            {
                uint8_t *d = event.packet->data;

                if (d[0] == 1) // player_update
                {
                    Vector2 pos;
                    memcpy(&pos.x, d + 1, 4);
                    memcpy(&pos.y, d + 5, 4);
                    PlayerDirection dir = static_cast<PlayerDirection>(d[9]);
                    bool idle = d[10] != 0;
                    float angle;
                    memcpy(&angle, d + 11, 4);

                    /*                     // Debug
                                        std::cout << "Recebido update: (" << pos.x << ", " << pos.y
                                                  << ") dir=" << (int)dir << " idle=" << idle
                                                  << " angle=" << angle << std::endl; */

                    if (isServer)
                    {
                        uint32_t id = (uint32_t)(uintptr_t)event.peer->data;
                        players[id].position = pos;
                        players[id].direction = dir;
                        players[id].isIdle = idle;
                        players[id].angle = angle;

                        // === IMPORTANTE: criar CÓPIA do pacote para broadcast ===
                        ENetPacket *copy = enet_packet_create(
                            event.packet->data,
                            event.packet->dataLength,
                            ENET_PACKET_FLAG_RELIABLE // ou 0 se quiser UDP, mas com cópia segura
                        );
                        // Envia para todos EXCETO o remetente
                        enet_peer_send(event.peer, 0, copy); // não usa broadcast, evita eco
                        // Ou se quiser broadcast mesmo: enet_host_broadcast(host, 0, copy);
                        // Mas tem que excluir o remetente manualmente se usar broadcast
                    }
                    else
                    {
                        // Cliente recebeu update de outro jogador → guardar em remote players
                        // (implementar depois)
                    }
                }
            }

            enet_packet_destroy(event.packet);
            break;
        }
        case ENET_EVENT_TYPE_DISCONNECT:
        {
            if (isServer)
            {
                uint32_t id = (uint32_t)(uintptr_t)event.peer->data;
                players.erase(id);
                std::cout << "Jogador " << id << " desconectou." << std::endl;
            }
            break;
        }
        }
    }
}

void Network::SendMyState(const Vector2 &pos, PlayerDirection dir, bool idle, float angle)
{
    if (!host)
        return;

    // Tamanho fixo: 1 (tipo) + 4+4 (pos) + 1 (dir) + 1 (idle) + 4 (angle) = 15 bytes
    constexpr size_t PACKET_SIZE = 15;
    char data[PACKET_SIZE];

    data[0] = 1; // player_update
    memcpy(data + 1, &pos.x, 4);
    memcpy(data + 5, &pos.y, 4);
    data[9] = static_cast<uint8_t>(dir);
    data[10] = idle ? 1 : 0;
    memcpy(data + 11, &angle, 4);

    ENetPacket *packet = enet_packet_create(data, PACKET_SIZE, 0); // UDP-like (não confiável = bom pra movimento)

    if (isServer)
        enet_host_broadcast(host, 0, packet); // servidor manda pra todos
    else
        enet_host_broadcast(host, 0, packet); // cliente só tem 1 peer (servidor)
}

void Network::Shutdown()
{
    if (host)
        enet_host_destroy(host);
    enet_deinitialize();
}
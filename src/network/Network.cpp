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
    myPlayerId = 0;   // servidor será ID 0
    nextPlayerId = 1; // clientes começam em 1
    players.clear();

    // Cria entrada para o próprio host (server)
    RemotePlayer &self = players[myPlayerId];
    self.peer = nullptr;

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
        myPlayerId = 0; // vai ser definido quando receber o pacote tipo 0 (assign_id)
        players.clear();
        return true;
    }

    enet_peer_reset(peer);
    return false;
}

void Network::Update(float /*delta*/)
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
                // Novo cliente conectado
                uint32_t id = nextPlayerId++;
                players[id].peer = event.peer;
                event.peer->data = (void *)(uintptr_t)id;

                // Envia ID ao cliente
                uint8_t packet[1 + 4]; // tipo + id
                packet[0] = 0;         // tipo: assign_id
                memcpy(packet + 1, &id, 4);

                ENetPacket *p = enet_packet_create(packet, sizeof(packet), ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(event.peer, 0, p);

                std::cout << "Novo jogador conectado. ID = " << id << std::endl;
            }
            break;
        }
        case ENET_EVENT_TYPE_RECEIVE:
        {
            uint8_t *d = event.packet->data;
            size_t len = event.packet->dataLength;

            if (len >= 1)
            {
                uint8_t type = d[0];

                // -----------------------------------
                // TIPO 0: assign_id (cliente recebe)
                // -----------------------------------
                if (type == 0)
                {
                    if (!isServer && len >= 5)
                    {
                        memcpy(&myPlayerId, d + 1, 4);
                        std::cout << "Meu ID recebido do servidor: " << myPlayerId << std::endl;

                        // Garante uma entrada para si mesmo (opcional)
                        RemotePlayer &self = players[myPlayerId];
                        self.peer = nullptr;
                    }
                }
                // -----------------------------------
                // TIPO 1: player_update
                // Pacote: [0]=tipo(1)
                //         [1..4]=id(uint32)
                //         [5..8]=pos.x
                //         [9..12]=pos.y
                //         [13]=dir
                //         [14]=idle
                //         [15..18]=angle
                // -----------------------------------
                else if (type == 1 && len >= 19)
                {
                    uint32_t id;
                    memcpy(&id, d + 1, 4);

                    Vector2 pos;
                    memcpy(&pos.x, d + 5, 4);
                    memcpy(&pos.y, d + 9, 4);

                    PlayerDirection dir = static_cast<PlayerDirection>(d[13]);
                    bool idle = d[14] != 0;

                    float angle;
                    memcpy(&angle, d + 15, 4);

                    if (isServer)
                    {
                        // Segurança: confia mais no peer->data do que no id do pacote
                        uint32_t peerId = (uint32_t)(uintptr_t)event.peer->data;
                        if (peerId != 0)
                            id = peerId;

                        RemotePlayer &st = players[id];
                        st.peer = event.peer;
                        st.position = pos;
                        st.direction = dir;
                        st.isIdle = idle;
                        st.angle = angle;

                        // Reenvia para TODOS (incluindo o remetente) o mesmo pacote
                        ENetPacket *copy = enet_packet_create(
                            event.packet->data,
                            event.packet->dataLength,
                            0); // movimento não precisa ser confiável

                        enet_host_broadcast(host, 0, copy);
                    }
                    else
                    {
                        // Cliente: atualiza o estado desse player
                        RemotePlayer &st = players[id];
                        st.position = pos;
                        st.direction = dir;
                        st.isIdle = idle;
                        st.angle = angle;
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

                // Opcional: avisar clientes que esse ID saiu (tipo 2, por ex.)
            }
            else
            {
                // Se algum dia você quiser tratar desconexão de outro player no cliente
                // pode apagar players[id] aqui usando alguma msg de "desconectado"
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

    // Se for cliente e ainda não recebeu o ID, não manda nada
    if (!isServer && myPlayerId == 0)
        return;

    constexpr size_t PACKET_SIZE = 19;
    uint8_t data[PACKET_SIZE];

    data[0] = 1; // player_update

    // ID do jogador (server usa 0, clientes usam myPlayerId)
    uint32_t idToSend = myPlayerId;
    memcpy(data + 1, &idToSend, 4);

    memcpy(data + 5, &pos.x, 4);
    memcpy(data + 9, &pos.y, 4);
    data[13] = static_cast<uint8_t>(dir);
    data[14] = idle ? 1 : 0;
    memcpy(data + 15, &angle, 4);

    ENetPacket *packet = enet_packet_create(data, PACKET_SIZE, 0); // não confiável (movimento)

    if (isServer)
    {
        // Host manda estado do próprio player para todos os clientes
        enet_host_broadcast(host, 0, packet);
    }
    else
    {
        // Cliente tem só 1 peer (servidor), broadcast funciona igual a "send pro server"
        enet_host_broadcast(host, 0, packet);
    }
}

void Network::Shutdown()
{
    if (host)
    {
        enet_host_destroy(host);
        host = nullptr;
    }
    enet_deinitialize();

    players.clear();
    myPlayerId = 0;
    isServer = false;
}

#include "Package.h"
#include "../common/Subject.h"
#include "any"
#include "memory"

class Context
{
public:
    std::unique_ptr<RemotePacket> receive;
    std::unique_ptr<RemotePacket> reply = nullptr;

    Context(std::unique_ptr<RemotePacket> receive) : receive(std::move(receive)) {}
    ~Context() = default;

    void SetReply(std::unique_ptr<RemotePacket> reply)
    {
        reply = std::move(reply);
    }
};

class NetworkClient : public Subject
{
public:
    NetworkClient() = default;
    ~NetworkClient() = default;

    void Handle(int32_t eventId, RemotePacket packet);
};

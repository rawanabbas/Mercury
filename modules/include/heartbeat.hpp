#ifndef HEARTBEAT_HPP
#define HEARTBEAT_HPP

#include "stdfax.h"
#include "udp_socket.hpp"
#include "thread.hpp"
#include "client.hpp"
#include "message.hpp"

class Heartbeat : public Client {
public:
    Heartbeat(std::string host, int port);
    virtual ~Heartbeat();

    void run();
};

#endif //HEARTBEAT_HPP

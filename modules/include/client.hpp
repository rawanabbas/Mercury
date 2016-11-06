#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "stdfax.h"
#include "udp_socket.hpp"
#include "thread.hpp"

class Client : public Thread {
private:

    static int _id;
    UDPSocket _sock;
    UDPSocket _serverSocket;
    char _buffer[MAX_RECV];

    void _execute();
    void _updateServerSocket(int port, std::string host);

public:
    Client (std::string hostname, int serverPort, int port = 0);
    virtual ~Client ();

    bool recvMsg(std::string &msg);
    bool sendMsg(std::string msg);
    void run();
};

#endif // CLIENT_HPP

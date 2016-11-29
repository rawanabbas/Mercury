#ifndef TRACKER_HPP
#define TRACKER_HPP

#include "thread.hpp"
#include "udp_socket.hpp"
#include "message.hpp"
#include "user.hpp"


class Tracker : public Thread {
    static int _id;
    void *_parent;
    User *_user;
    UDPSocket _clientSocket;
    UDPSocket _serverSocket;
    Message _msg;

    bool _sendInfo();
    void _pulse();
public:

    Tracker(UDPSocket clientSocket, User *user);

    void run();
    std::string getUserID();
    inline bool operator == (const Tracker& tracker);

    virtual ~Tracker();

};

#endif //TRACKER_HPP

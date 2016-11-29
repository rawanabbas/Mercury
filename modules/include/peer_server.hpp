#ifndef PEER_SERVER_HPP
#define PEER_SERVER_HPP

#include "stdfax.h"
#include "udp_socket.hpp"
#include "user.hpp"
#include "database.hpp"
#include "thread.hpp"
#include "tracker.hpp"

class PeerServer : public Thread {

    static const std::string UsernameToken;
    static const std::string PasswordToken;

    Database _db;
    UDPSocket _sock;
    std::map<std::string, User*> _peers;
    std::vector<Tracker*> _trackers;

    void _terminateTracker(Tracker *tracker);
    static void * _callbackWrapper(Thread * thread, void * parent);

    bool _authenticate(Message &msg, User *user);
    User _parseAuthenticationMessage(Message &msg);
    void _spawnTracker(UDPSocket clientSocket, User *user);


public:
    PeerServer(int port);
    void run();

    virtual ~PeerServer();
    bool _register(Message msg, User *user);
};

#endif //PEER_SERVER_HPP

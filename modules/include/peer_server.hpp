#ifndef PEER_SERVER_HPP
#define PEER_SERVER_HPP

#include "stdafx.h"
#include "udp_socket.hpp"
#include "user.hpp"
#include "peer.hpp"
#include "database.hpp"
#include "thread.hpp"
#include "tracker.hpp"


class PeerServer : public Thread {

    static const std::string UsernameToken;
    static const std::string PasswordToken;

    Database _db;
    UDPSocket _sock;
    PeerMap _peers;
    std::vector<Tracker*> _trackers;

    pthread_mutex_t _peersMutex;

    void _terminateTracker(Tracker *tracker);
    static void * _callbackWrapper(Thread * thread, void * parent);

    void _spawnTracker(UDPSocket clientSocket, User *user);

    User _parseAuthenticationMessage(Message &msg);

    bool _authenticate(Message &msg, std::string clientHost);
    bool _register(Message msg, User *user);

    void _addPeer(User *user);

public:
    PeerServer(int port);
    void run();

    PeerMap getPeers() const;
    pthread_mutex_t getPeersMutex() const;

    virtual ~PeerServer();
};

#endif //PEER_SERVER_HPP

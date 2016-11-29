#ifndef HEARTBEAT_HPP
#define HEARTBEAT_HPP

#include "stdafx.h"
#include "udp_socket.hpp"
#include "thread.hpp"
#include "client.hpp"
#include "message.hpp"
#include "user.hpp"

typedef std::map<std::string, Peer*> PeerMap;

enum class Status {
    EstablishingConnection,
    Ping,
    Waiting,
    Failed,
    Pong
};

class Heartbeat : public Client {

    pthread_condattr_t _timerAttr;
    pthread_mutex_t _timerMutex;
    pthread_cond_t _timerConditional;

    PeerMap _peers;

    Status _status;

    timespec _pingTime;
    int _retry;

    bool _query;

    void _wait(long pingTime);
    void _resetTrials();

    bool _establishConnection();
    void _queryPeers();

    void _parsePeerList(std::string peerList);

public:

    Heartbeat(std::string ownerId, std::string host, int port);

    Status getStatus();

    void queryPeers();
    PeerMap getPeers();

    void run();

    virtual ~Heartbeat();


};

#endif //HEARTBEAT_HPP

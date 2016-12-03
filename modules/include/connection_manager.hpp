#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include "stdafx.h"
#include "peer.hpp"
#include "thread.hpp"
#include "udp_socket.hpp"
#include "message.hpp"
#include "client.hpp"
#include "heartbeat.hpp"

typedef std::map<std::string, Peer*> PeerMap;

enum class ManagerStatus {
    FetchingPeers,
    FetcheedPeers,
    Connected,
    Error
};

class ConnectionManager : public Heartbeat {


    ManagerStatus _mStatus;

    PeerMap _peers;
    Message _queryMessage;
    pthread_mutex_t _peerMutex;

    void _queryPeers();
    void _parsePeerList(std::string peerList);

public:

    ConnectionManager(std::string ownerId, std::string username, std::string host, int port);

    Peer getPeer(std::string userID);
    PeerMap getPeers();

    ManagerStatus getStatus();

    void run();

    virtual ~ConnectionManager();

};

#endif //CONNECTION_MANAGER_HPP

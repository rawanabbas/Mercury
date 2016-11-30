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

class ConnectionManager : public Heartbeat {


    PeerMap _peers;
    Message _queryMessage;

    void _queryPeers();
    void _parsePeerList(std::string peerList);

public:

    ConnectionManager(std::string ownerId, std::string host, int port);

    Peer getPeer(std::string userID);
    void run();

    virtual ~ConnectionManager();

};

#endif //CONNECTION_MANAGER_HPP

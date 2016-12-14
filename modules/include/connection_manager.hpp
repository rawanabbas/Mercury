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

#include <QObject>
#include <QDebug>

enum class ManagerStatus {
    FetchingPeers,
    FetcheedPeers,
    Connected,
    Error
};

class ConnectionManager : public QObject, public Heartbeat {

     Q_OBJECT

    ManagerStatus _mStatus;

    PeerMap _peers;
    std::vector<Peer*> _allPeers;

    Message _queryMessage;

    pthread_mutex_t _peerMutex;

    void _queryPeers();
    void _parsePeerList(std::string peerList);

 signals:

     void peersUpdated();

public:


    ConnectionManager(std::string ownerId, std::string username, std::string host, int port);

    bool isPeerOnline(std::string userID);
    PeerMap getPeers();

    ManagerStatus getStatus();

    void run();


    void _fetchAllPeers();

    std::vector<Peer *> getAllPeers() const;
    void setAllPeers(const std::vector<Peer *> &allPeers);

    virtual ~ConnectionManager();
signals:
    void disconnected();
    void connected();
};

#endif //CONNECTION_MANAGER_HPP

#include "connection_manager.hpp"



ConnectionManager::ConnectionManager(std::string ownerId, std::string username, std::string host, int port) : QObject(0), Heartbeat(ownerId, username, host, port) {

    qDebug() << "Connection Manager";

    _queryMessage.setMessageType(MessageType::Query);
    _queryMessage.setMessage("Query");
    _queryMessage.addHeader(Message::OwnerIdToken, ownerId);

    pthread_mutex_init(&_peerMutex, NULL);
    setMutex(&_peerMutex);
    _mStatus = ManagerStatus::FetchingPeers;
}

bool ConnectionManager::isPeerOnline(std::string userID) {

    return _peers.count(userID) != 0;

}

PeerMap ConnectionManager::getPeers() {

    return _peers;

}

ManagerStatus ConnectionManager::getStatus() {

    return _mStatus;

}

void ConnectionManager::_fetchAllPeers() {

//    qDebug() << "Fetching all users!";
    Message allPeers(_ownerId, _username, "All", MessageType::QueryAll, RPC::Undefined);

    if (!_sendMessage(allPeers)) {

        std::cerr << "Cannot send query message!" << std::endl;

    } else {

        std::string peers;

        if (_receiveWithTimeout(peers, 5) == -1) {

            std::cerr << "Cannot fetch all peers!" << std::endl;

        } else {
            _allPeers.clear();

            allPeers = Message::deserialize(peers);

            peers = allPeers.getMessage();

            std::stringstream ss(peers);
            std::string peerToken;

            while(ss >> peerToken) {

//                qDebug() << QString::fromStdString(peerToken);

                Peer *peer = new Peer;

                std::stringstream ssPeer(peerToken);
                std::string username, userID;


                std::getline(ssPeer, userID, ',');
                std::getline(ssPeer, username, ';');

                if (username != _username) {

                    peer->setUserID(userID);
                    peer->setUsername(username);

                    _allPeers.push_back(peer);
                }

            }

        }

        qDebug() << "Peers Fetched!";
    }
}

void ConnectionManager::run() {

    int connectionRetires = 0;
    int maxConnectionRetires = 100;

    while (connectionRetires < maxConnectionRetires) {
        std::cout << "Attempting to connect (CM)." << std::endl;
        _retry = 3;

        if (!_establishConnection()) {

            std::cerr << "Cannot connect to the peering server!" << std::endl;

        } else {

            _fetchAllPeers();

            while(_retry > 0) {


                if (!_sendMessage(_queryMessage)) {

                    perror("Cannot Send Query!");
                    continue;

                } else {

                    std::string info;
                    if (_receiveWithTimeout(info, 2) == -1) {

                        perror("TimedOut!");
                        _retry--;
                        break;

                    } else {

                        Message result = Message::deserialize(info);

                        if (result.getMessageType() == MessageType::Result) {

                            _parsePeerList(result.getMessage());

                            _mStatus = ManagerStatus::FetcheedPeers;

                            emit peersUpdated();

                        } else {
                            std::cout << (int)result.getMessageType() << std::endl;
                            std::cerr << "Undefined response." << std::endl;
                            _mStatus = ManagerStatus::Error;


                        }


                    }


                }


                _resetTrials();
                _wait(5);

            }
        }
    }


}


std::vector<Peer *> ConnectionManager::getAllPeers() const
{
    return _allPeers;
}

void ConnectionManager::setAllPeers(const std::vector<Peer *> &allPeers)
{
    _allPeers = allPeers;
}
void ConnectionManager::_parsePeerList(std::string peerList) {

    std::stringstream ss(peerList);
    std::string peerStr;

    //    std::cout << peerList << std::endl;

    while (ss >> peerStr) {

        std::string userID, ip, username;
        std::istringstream ssPeer(peerStr);

        std::getline(ssPeer, userID, ',');
        std::getline(ssPeer, username, ',');
        std::getline(ssPeer, ip);

        if (userID != _ownerId) {

//            qDebug() << "**************************************************";
//            qDebug() << "userID: " << QString::fromStdString(userID);
//            qDebug() << "_ownerID: " << QString::fromStdString(_ownerId);
//            qDebug() << "**************************************************";

            Peer *peer = new Peer(userID, username, ip);

            lock();
            _peers[userID] = peer;
            release();

        } /*else {

            _peers.erase(userID);

        }*/


    }
}

ConnectionManager::~ConnectionManager() {

}

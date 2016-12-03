#include "connection_manager.hpp"



ConnectionManager::ConnectionManager(std::string ownerId, std::string username, std::string host, int port) : Heartbeat(ownerId, username, host, port) {

    _queryMessage.setMessageType(MessageType::Query);
    _queryMessage.setMessage("Query");
    _queryMessage.addHeader(Message::OwnerIdToken, ownerId);

    pthread_mutex_init(&_peerMutex, NULL);
    setMutex(_peerMutex);
    _mStatus = ManagerStatus::FetchingPeers;
}

Peer ConnectionManager::getPeer(std::string userID) {

    return *_peers[userID];

}

PeerMap ConnectionManager::getPeers() {

    while(_mStatus !=  ManagerStatus::FetcheedPeers);

    return _peers;

}

ManagerStatus ConnectionManager::getStatus() {

    return _mStatus;

}

void ConnectionManager::run() {

    if (!_establishConnection()) {

        std::cerr << "Cannot connect to the peering server!" << std::endl;

    } else {

        while(true) {


            if (!_sendMessage(_queryMessage)) {

                perror("Cannot Send Query!");
                continue;

            } else {

                std::string info;
                if (_receiveWithTimeout(info, 5) == -1) {

                    perror("TimedOut!");
                    continue;

                } else {

                    Message result = Message::deserialize(info);

                    if (result.getMessageType() == MessageType::Result) {

                        _parsePeerList(result.getMessage());

                        _mStatus = ManagerStatus::FetcheedPeers;

                    } else {

                        std::cerr << "Undefined response." << std::endl;
                        _mStatus = ManagerStatus::Error;


                    }


                }


            }


            _resetTrials();
            _wait(20);

        }
    }


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

            Peer *peer = new Peer(userID, username, ip);

            lock();
            _peers[userID] = peer;
            release();

        } else {

            _peers.erase(userID);

        }


    }
}

ConnectionManager::~ConnectionManager() {

}

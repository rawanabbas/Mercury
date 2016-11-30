#include "connection_manager.hpp"



ConnectionManager::ConnectionManager(std::string ownerId, std::string host, int port) : Heartbeat(ownerId, host, port) {

    _queryMessage.setMessageType(MessageType::Query);
    _queryMessage.setMessage("Query");
    _queryMessage.addHeader("Owner-Id", ownerId);

    pthread_mutex_init(_peerMutex, NULL);
    setMutex(_peerMutex);

}

Peer ConnectionManager::getPeer(std::string userID) {

    return *_peers[userID];

}

void ConnectionManager::run() {

    while(true) {

        if (!_establishConnection()) {
            std::cerr << "Cannot connect to the peering server!" << std::endl;
        }

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

                } else {

                    std::cerr << "Undefined response." << std::endl;


                }


            }


        }


        _resetTrials();
        _wait(20);

    }

}

void ConnectionManager::_parsePeerList(std::string peerList) {

    std::stringstream ss(peerList);
    std::string peerStr;

    std::cout << peerList << std::endl;

    while (ss >> peerStr) {

        std::string userID, ip, username;
        std::istringstream ssPeer(peerStr);

        std::getline(ssPeer, userID, ',');
        std::getline(ssPeer, username, ',');
        std::getline(ssPeer, ip);

        Peer *peer = new Peer(userID, ip, username);

        lock();
        _peers[userID] = peer;
        release();

    }
}

ConnectionManager::~ConnectionManager() {
    
}

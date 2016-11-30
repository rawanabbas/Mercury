/**
 * class Message
 * This class is meant to for sending pings to clients as TCP/IP Keep Alive Protocol.
 * Author: Group 3
 * Date: 13th November 2016
 *  Project One - CSCE 4411
 */
#include "heartbeat.hpp"

void Heartbeat::_wait(long pingTime) {

    clock_gettime(CLOCK_MONOTONIC, &_pingTime);
    _pingTime.tv_sec += pingTime;
    pthread_cond_timedwait(&_timerConditional, &_timerMutex, &_pingTime);

}

void Heartbeat::_resetTrials() {

    _retry = MAX_RETRY;

}

Heartbeat::Heartbeat(std::string ownerId, std::string host, int port) : Client(ownerId, host, port), _query(false) {

    pthread_condattr_init(&_timerAttr);
    pthread_condattr_setclock(&_timerAttr, CLOCK_MONOTONIC);

    pthread_mutex_init(&_timerMutex, NULL);
    pthread_cond_init(&_timerConditional, &_timerAttr);

    _resetTrials();

}

PeerMap Heartbeat::getPeers() {
    return _peers;
}

void Heartbeat::_parsePeerList(std::string peerList) {

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

        _peers[userID] = peer;

    }
}

void Heartbeat::_queryPeers() {

    Message queryMessage(getOwnerId(), "Query!", MessageType::Query);

    if (!_sendMessage(queryMessage)) {

        perror("Cannot Send Messages!");
        std::cerr << "An error has occured, cannot send messages!" << std::endl;

    } else {

        std::string sPeerList;

        if (_receive(sPeerList) == -1) {

            perror("Cannot recieve the peering list!");
            std::cerr << "An error has occured, cannot recieve the peering list!" << std::endl;

        } else {

            std::cout << "Results recieved!" << std::endl;

            Message resultMessage = Message::deserialize(sPeerList);

            if (resultMessage.getMessageType() == MessageType::Result) {

                std::string peerList = resultMessage.getMessage();

                _parsePeerList(peerList);

            } else {

                std::cout << "Cannot understand response!" << std::endl;
            }
        }

    }
}

void Heartbeat::queryPeers() {
    _query = true;
}

bool Heartbeat::_establishConnection() {

    Message eMessage(getOwnerId(), "Establish!", MessageType::EstablishConnection);

    while(_retry > 0) {

        _status = Status::EstablishingConnection;

        if(!_sendMessage(eMessage)) {

            _retry--;

        } else {

            _resetTrials();
            break;

        }
    }
    if (_retry <= 0) {

        std::cout << "Failed To Establish Connection!" << std::endl;
        return false;

    } else {

        std::string info;

        if (_receiveWithTimeout(info) == -1) {

            _status = Status::Failed;
            perror("Cannot Recieve Message!");
            std::cout << "Failed To Recieve Connection Info, Now Exiting!" << std::endl;
            return false;

        } else {
            Message message = Message::deserialize(info);

            if (message.getMessageType() == MessageType::Info) {

                _updateServerSocket(atoi(message.getMessage().c_str()), _serverSocket.getHost());
                return true;

            } else {

                std::cout << "Unkown Message Type: " << (int) message.getMessageType() << std::endl;
                _status = Status::Failed;
                return false;
            }
        }
    }
}

void Heartbeat::run() {

    if (_establishConnection()) {

        while(_retry > 0) {

            if (!_query) {

                Message pingMessage(getOwnerId(), "Ping", MessageType::Ping);

                if (!_sendMessage(pingMessage)) {

                    perror("Cannot Send Message!");
                    _retry--;

                } else {


                    std::string msg;

                    int r;

                    while (((r = _receiveWithTimeout(msg, 5)) == -1) && (_retry > 0)) {
                        std::cout << "Cannot recieve retrying!" << std::endl;
                        _retry--;
                    }

                    if (r == -1 && _retry == 0) {

                        perror("Cannot Recieve Message!");
                        break;

                    }

                }

            } else {

                _queryPeers();
                _query = false;

            }

            _resetTrials();
            _wait(2);

        }

    }

    std::cout << "Heartbeat Stopped, Server " << _sock.getHost() << ":" << _sock.getPortNumber() << " is unreachable" << std::endl;

}

Heartbeat::~Heartbeat() {

    std::cout << "~Heartbeat" << std::endl;

}

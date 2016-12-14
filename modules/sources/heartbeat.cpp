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

Heartbeat::Heartbeat(std::string ownerId, std::string username, std::string host, int port) : Client(ownerId, username, host, port) {

    pthread_condattr_init(&_timerAttr);
    pthread_condattr_setclock(&_timerAttr, CLOCK_MONOTONIC);

    pthread_mutex_init(&_timerMutex, NULL);
    pthread_cond_init(&_timerConditional, &_timerAttr);

    _resetTrials();

}

bool Heartbeat::_establishConnection() {
    _updateServerSocket(3010, this->getHost());
    Message eMessage(getOwnerId(), getUsername(), "Establish!", MessageType::EstablishConnection);

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
    int connectionRetires = 0;
    int maxConnectionRetires = 100;

    while (connectionRetires < maxConnectionRetires) {
        std::cout << "Attempting to connect." << std::endl;
        _retry = 3;
        if (_establishConnection()) {

            while(_retry > 0) {

                Message pingMessage(getOwnerId(), getUsername(), "Ping", MessageType::Ping);

                if (!_sendMessage(pingMessage)) {

                    perror("Cannot Send Message!");
                    _retry--;

                } else {


                    std::string msg;

                    int r;

                    while (((r = _receiveWithTimeout(msg, 2)) == -1) && (_retry > 0)) {
                        std::cout << "Cannot recieve retrying!" << std::endl;
                        _retry--;
                    }

                    if (r == -1 && _retry == 0) {

                        perror("Cannot Recieve Message!");
                        break;

                    }

                }


                _resetTrials();
                _wait(2);

            }

        }
    }

    std::cout << "Heartbeat Stopped, Server " << _sock.getHost() << ":" << _sock.getPortNumber() << " is unreachable" << std::endl;

}

Heartbeat::~Heartbeat() {

//    std::cout << "~Heartbeat" << std::endl;

}

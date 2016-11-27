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

Heartbeat::Heartbeat(std::string host, int port) : Client(host, port, 0) {

    pthread_condattr_init(&_timerAttr);
    pthread_condattr_setclock(&_timerAttr, CLOCK_MONOTONIC);
    pthread_mutex_init(&_timerMutex, NULL);
    pthread_cond_init(&_timerConditional, &_timerAttr);
    _resetTrials();

}

bool Heartbeat::_establishConnection() {

    Message eMessage("Establish!", MessageType::EstablishConnection);
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

                std::cout << "Updated Server Socket Info: " << message.getMessage() << std::endl;
                _updateServerSocket(atoi(message.getMessage().c_str()), _serverSocket.getHost());
                return false;

            } else {

                std::cout << "Unkown Message Type: " << (int) message.getMessageType() << std::endl;
                _status = Status::Failed;
                return false;
            }
        }
    }
}

void Heartbeat::run() {

    if (!_establishConnection()) {

        while(true) {

            Message pingMessage("Ping", MessageType::Ping);

            if (!_sendMessage(pingMessage)) {

                perror("Cannot Send Message!");

            } else {

                std::string msg;
                std::cout << "Ping" << std::endl;

                if (_receiveWithTimeout(msg) == -1) {

                    perror("Cannot Recieve Message!");
                    std::cerr << "Server Disconnected!" << std::endl;
                    break;

                } else {

                    std::cout << "Pong!" << std::endl;

                }

            }

            _wait(2);

        }

    }

    std::cout << "Heartbeat Stopped, Server " << _serverSocket.getHost() << ":" << _serverSocket.getPortNumber() << " is unreachable" << std::endl;

}

Heartbeat::~Heartbeat() {

    std::cout << "~Heartbeat" << std::endl;

}

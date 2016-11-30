#include "tracker.hpp"

int Tracker::_id = 0;

Tracker::Tracker(UDPSocket clientSocket, User *user, std::map<std::string, Peer *> *peers) : Thread(), _clientSocket(clientSocket), _user(user), _peers(peers) {

}

void Tracker::run() {

    _isRunning = true;

    std::cout << "Handling Client " << _clientSocket.getHost() << ": " << _clientSocket.getPortNumber() << " from thread -> " << getThreadId() << std::endl;

    if (!_clientSocket.isValid()) {

        perror("Invalid Client Socket.");
        return;

    } else if (!_serverSocket.create()) {

        perror("Couldn't create a new server socket!");
        return;

    } else if (!_sendInfo()) {

        perror("Cannot send the info!");
        return;

    } else {

        _pulse();

    }
}

std::string Tracker::getUserID() {

    return _user->getUserID();

}

Tracker::~Tracker() {

    delete _user;

}

bool Tracker::operator == (const Tracker &tracker) {

    return tracker._id == _id;

}

bool Tracker::_sendInfo() {

    if (!_serverSocket.sendMessageTo(_clientSocket, Message(_user->getUserID(), std::to_string(_serverSocket.getPortNumber()), MessageType::Info))) {

        perror("Cannot Send the New Socket!");
        return false;

    } else {

        std::cout << "Sending: " << std::to_string(_serverSocket.getPortNumber()) << std::endl;
        return true;

    }
}

void Tracker::_pulse() {

    std::cout << "Heartbeating on " << _serverSocket.getPortNumber() << std::endl;
    std::string serializedMsg;

    while(true) {

        if (_serverSocket.recvWithTimeout(_clientSocket, serializedMsg, 5) != -1) {
            _msg = Message::deserialize(serializedMsg);

            if(_msg.getMessageType() == MessageType::Ping) {

                _msg.setMessage("Pong!");
                _msg.setMessageType(MessageType::Pong);

                if (!_serverSocket.sendMessageTo(_clientSocket, _msg)) {

                    perror("Cannot send Pong!");
                    break;
                }

            } else if (_msg.getMessageType() == MessageType::Exit) {

                break;

            } else if (_msg.getMessageType() == MessageType::Query) {
                //TODO
                lock();

                std::string peerList = "";
                std::map<std::string, Peer*>::iterator it;

                for (it = _peers->begin(); it != _peers->end(); it++) {

                    peerList += it -> first + "," + it -> second -> getUsername() + "," + it -> second -> getIP() + "\n";

                }


                release();

                std::cout << "Peer-List: " << peerList << std::endl;

                _msg.setMessageType(MessageType::Result);
                _msg.addHeader("Owner-Id: ", "Tracker-" + std::to_string(getThreadId()));
                _msg.setMessage(peerList);

                if (!_serverSocket.sendMessageTo(_clientSocket, _msg)) {

                    perror("Cannot send peer list! Client may be offline.");

                }

            }

        } else {

            perror("Cannot recieve new requests!");
            break;

        }
    }

    std::cout << "Beating has stopped for " << _serverSocket.getHost() << ":" << _serverSocket.getPortNumber() << std::endl;
}

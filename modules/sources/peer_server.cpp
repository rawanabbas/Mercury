#include "peer_server.hpp"

const std::string PeerServer::UsernameToken = "username:";
const std::string PeerServer::PasswordToken = "password:";

PeerServer::PeerServer(int port) {

    if (!_sock.create(port) || _db.getStatus() == DatabaseStatus::Error) {

        throw "An error has occured while creating the server socket.";

    }
    if (pthread_mutex_init(&_peersMutex, NULL)) {

        throw "An error has occured while initializing the mutex";

    }

    _isRunning = true;
}

User PeerServer::_parseAuthenticationMessage(Message &msg) {
    User user;

    std::stringstream ss(msg.getMessage());
    std::string token;
    while(ss >> token) {
        if (token == UsernameToken) {

            std::string username;
            ss >> username;
            user.setUsername(username);

        } else if (token == PasswordToken) {

            std::string password;
            ss >> password;
            user.setPassword(password);

        }
    }

    return user;
}

bool PeerServer::_authenticate(Message &msg, User *user) {

    User aUser = _parseAuthenticationMessage(msg);
    User authUser = _db.fetch(aUser);

    if (aUser.isAuthenticated(authUser)) {

        user = new User(aUser);
        msg.setMessage(user->getUserID());
        msg.setMessageType(MessageType::Authenticated);
        return true;

    } else {

        user = nullptr;
        msg.setMessage("Authentication Error!");
        msg.setMessageType(MessageType::Unauthorized);
        return false;

    }
}

void PeerServer::_spawnTracker(UDPSocket clientSocket, User *user) {

    Tracker *tracker = new Tracker(clientSocket, user, &_peers);
    tracker -> setParent((void *)this);
    tracker -> setDoneCallback(_callbackWrapper, (void *)this);
    tracker -> setMutex(_peersMutex);
    tracker -> start();

    _trackers.push_back(tracker);

    user->setIP(clientSocket.getHost());
    user->setPort(3001);

}

void PeerServer::_addPeer(User *user) {
    lock();
    _peers[user->getUserID()] = (Peer *)user;
    release();
}

bool PeerServer::_register(Message msg, User *user) {

    std::stringstream ss(msg.getMessage());
    std::string token;

    std::string username, password;


    while(ss >> token) {

        if (token == UsernameToken) {

            ss >> username;

        } else if (token == PasswordToken){

            ss >> password;

        }

    }

    _addPeer(user);
    user->setPassword(password);
    user->setUsername(username);


    std::cout << "UserID: " << user->getUserID() << std::endl;
    return _db.insert(*user);
}

void PeerServer::run() {

    std::cout << "Peer Server listening on " << _sock.getPortNumber() << std::endl;

    while (_isRunning) {

        UDPSocket clientSocket;
        std::string serializedMsg;
        Message msg;

        if (_sock.recvFrom(clientSocket, serializedMsg) != -1) {

            std::cout << "Client Socket: " << clientSocket.getPortNumber() << std::endl;
            std::cout << "Messaged Received From  " << clientSocket.getHost() << ":" << clientSocket.getPortNumber() << "-> " << serializedMsg << std::endl;

            msg = Message::deserialize(serializedMsg);

            if (msg.getMessageType() == MessageType::Authenticate) {

                User *user;

                if(_authenticate(msg, user) && user != nullptr) {

                    _addPeer(user);

                }

                _sock.sendMessageTo(clientSocket, msg);


            } else if (msg.getMessageType() == MessageType::Register){

                User *user = new User;

                if (_register(msg, user)) {

                    _addPeer(user);

                    msg.setMessage(user->getUserID());
                    msg.setMessageType(MessageType::Authenticated);

                } else {

                    msg.setMessage("Error!");
                    msg.setMessageType(MessageType::Unauthorized);

                }

                _sock.sendMessageTo(clientSocket, msg);

            } else if (msg.getMessageType() == MessageType::EstablishConnection) {

                User *user = new User;
                user->setUserID(msg.getHeader("Owner-Id: "));
                _spawnTracker(clientSocket, user);

            }
        }

    }
}


PeerMap PeerServer::getPeers() const {
    return _peers;
}

pthread_mutex_t PeerServer::getPeersMutex() const {
    return _peersMutex;
}

void PeerServer::_terminateTracker(Tracker *tracker) {

    std::cout << "Terminating the Tracker!" << std::endl;

    _isRunning = false;

    std::vector<Tracker*>::iterator it = std::find(_trackers.begin(), _trackers.end(), tracker);

    if (it != _trackers.end()) {

        std::string userID = (*it) -> getUserID();
        (*it) -> join();

        _trackers.erase(it);

        lock();
        _peers.erase(userID);
        release();

    }
}

void *PeerServer::_callbackWrapper(Thread *thread, void *parent) {

    Tracker *tracker = (Tracker *) thread;
    PeerServer *peerServer = (PeerServer *) parent;
    peerServer -> _terminateTracker(tracker);
    return (void *) thread;

}

PeerServer::~PeerServer() {

}

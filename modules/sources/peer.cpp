#include "peer.hpp"

int Peer::getPort() const {
    return _port;
}

void Peer::setPort(int value) {
    _port = value;
}

std::string Peer::getIp() const {
    return _ip;
}

void Peer::setIp(const std::string &value) {
    _ip = value;
}

std::string Peer::getUsername() const {
    return _username;
}

void Peer::setUsername(const std::string &value) {
    _username = value;
}

std::string Peer::getPassword() const {
    return _password;
}

void Peer::setPassword(const std::string &value) {
    _password = value;
}

Peer::Peer() {
    
}

Peer::~Peer() {
    
}

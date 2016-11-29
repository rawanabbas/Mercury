#include "peer.hpp"



Peer::Peer() {

}

Peer::Peer(std::string username, std::string ip, int port)
    : _username(username), _ip(ip), _port(port) {

    _generateID();

}

Peer::Peer(std::string id, std::string username, std::string ip, int port)
    : _username(username), _ip(ip), _port(port), _id(id) {

}

int Peer::getPort() const {
    return _port;
}

void Peer::setPort(int value) {
    _port = value;
}

std::string Peer::getIP() const {
    return _ip;
}

void Peer::setIP(const std::string &value) {
    _ip = value;
}

std::string Peer::getUsername() const {
    return _username;
}

void Peer::setUsername(const std::string &value) {
    _username = value;
}



std::string Peer::getUserID() const {
    return _id;
}

void Peer::setUserID(const std::string &id) {
    _id = id;
}

void Peer::_generateID() {

    CryptoPP::SecByteBlock randomID(CryptoPP::AES::BLOCKSIZE);
    std::string id, hashedUsername;

    CryptoPP::SHA256 hash;
    CryptoPP::StringSource(_username, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hashedUsername))));


    OS_GenerateRandomBlock(false, randomID, randomID.size());
    CryptoPP::HexEncoder hex(new CryptoPP::StringSink(id));
    hex.Put(randomID, randomID.size());
    hex.MessageEnd();


    _id = hashedUsername + id;

}
Peer::~Peer() {
    
}

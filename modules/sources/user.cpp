#include "user.hpp"

User::User() {

}

User::User(std::string username, std::string password, std::string ip, int port) : _username(username), _ip(ip), _port(port){

    _hashPassword(password);
    _generateID();

}

std::string User::getUserID() {

    return _id;

}

std::string User::getUsername() {

    return _username;

}

std::string User::getPassword() {

    return _password;

}

std::string User::getIP() {

    return _ip;

}

int User::getPort() {
    return _port;
}

time_t User::getLastLogin() {

    return _lastLogin;

}

void User::setUserID(std::string userID) {

    _id = userID;

}

void User::setUsername(std::string username) {

    _username = username;
    _generateID();

}

void User::setPassword(std::string password) {

    _hashPassword(password);

}

void User::setPasswordWHash(std::string password) {
    _password = password;
}

void User::setIP(std::string ip) {

    _ip = ip;

}

void User::setPort(int port) {

    _port = port;

}

void User::setLastLogin(time_t lastLogin) {

    _lastLogin = lastLogin;

}


bool User::isAuthenticated(User user) {
    std::cout << "My password: " << getPassword() << std::endl;
    std::cout << "Compared Password: " << user.getPassword() << std::endl;
    return (user.getUsername() == getUsername() && user.getPassword() == getPassword());

}

void User::_hashPassword(std::string password) {

    CryptoPP::SHA256 hash;
    CryptoPP::StringSource(password, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(_password))));

}

void User::_generateID() {

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

User::~User() {

}

#include "user.hpp"

User::User() {

}

User::User(std::string username, std::string password, std::string ip, int port) : Peer(username, ip, port){

    _hashPassword(password);

}

std::string User::getPassword() {

    return _password;

}

time_t User::getLastLogin() {

    return _lastLogin;

}

void User::setPassword(std::string password) {

    _hashPassword(password);

}

void User::setPasswordWHash(std::string password) {
    _password = password;
}

void User::signup(std::string username, std::string password) {

    setUsername(username);
    setPassword(password);
    _generateID();

}


void User::setLastLogin(time_t lastLogin) {

    _lastLogin = lastLogin;

}


bool User::isAuthenticated(User user) {

    return (user.getUsername() == getUsername() && user.getPassword() == getPassword());

}

void User::_hashPassword(std::string password) {

    CryptoPP::SHA256 hash;
    CryptoPP::StringSource(password, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(_password))));

}

User::~User() {

}

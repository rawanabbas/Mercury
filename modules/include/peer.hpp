#ifndef PEER_HPP
#define PEER_HPP

#include "stdfax.h"

class Peer {
private:
    int _port;
    std::string _ip;
    std::string _username;
    std::string _password;

    std::string _hash();

public:
    Peer ();

    virtual ~Peer ();

    int getPort() const;
    void setPort(int value);

    std::string getIp() const;
    void setIp(const std::string &value);

    std::string getUsername() const;
    void setUsername(const std::string &value);

    std::string getPassword() const;
    void setPassword(const std::string &value);
};

#endif //PEER_HPP

#ifndef PEER_HPP
#define PEER_HPP

#include "stdafx.h"

class Peer {

protected:

    int _port;
    std::string _ip;
    std::string _id;
    std::string _username;

    void _generateID();

public:

    Peer();
    Peer(std::string username, std::string ip, int port = 3001);
    Peer(std::string id, std::string username, std::string ip, int port = 3001);


    int getPort() const;
    void setPort(int value);

    std::string getIP() const;
    void setIP(const std::string &value);

    std::string getUsername() const;
    void setUsername(const std::string &value);

    std::string getUserID() const;
    void setUserID(const std::string &id);

    virtual ~Peer ();

};

#endif //PEER_HPP

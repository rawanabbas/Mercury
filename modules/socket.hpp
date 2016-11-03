#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "stdfax.h"



class Socket {
private:
    int _sock;
    sockaddr_in _address;
public:
    //Constructor & Destructor
    Socket ();
    virtual ~Socket ();
    //Methods
    bool create();
    bool bind(const int port = 0);
    bool listen() const;
    bool accept(Socket&) const;

    int getPortNumber();
    std::string getHost();

    bool connect(const std::string host, const int port);
    bool send(const std::string msg) const;
    bool sendTo(std::string host, int port, std::string msg);
    int recv(std::string& msg) const;

    void setNonBlocking(const bool block);
    bool isValid() const;

};

#endif // SOCKET_HPP

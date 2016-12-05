#ifndef USER_HPP
#define USER_HPP

#include "stdafx.h"
#include "peer.hpp"

class User : public Peer {

    std::string _password;

    time_t _lastLogin;


    void _hashPassword(std::string password);

public:

    User();
    User(std::string username, std::string password, std::string ip = "0.0.0.0", int port = 3001);

    std::string getPassword();
    time_t getLastLogin();


    void setPassword(std::string password);
    void setPasswordWHash(std::string password);

    void signup(std::string username, std::string password);

    void setLastLogin(time_t lastLogin);
    bool isAuthenticated(User user);

    virtual ~User();

};

#endif //USER_HPP


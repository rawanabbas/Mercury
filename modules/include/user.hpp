#ifndef USER_HPP
#define USER_HPP

#include "stdfax.h"

class User {

    std::string _id;
    std::string _username;
    std::string _password;
    std::string _ip;
    int _port;
    time_t _lastLogin;

    void _hashPassword(std::string password);
    void _generateID();

public:

    User();
    User(std::string username, std::string password, std::string ip = "0.0.0.0", int port = 0);

    std::string getUserID();
    std::string getUsername();
    std::string getPassword();
    std::string getIP();
    int getPort();
    time_t getLastLogin();

    void setUserID(std::string userID);
    void setUsername(std::string username);
    void setPassword(std::string password);
    void setPasswordWHash(std::string password);
    void setIP(std::string ip);
    void setPort(int port);
    void setLastLogin(time_t lastLogin);

    bool isAuthenticated(User user);

    void updateIP(std::string ip);
    void updateLastLogin(time_t lastLogin);

    virtual ~User();

};

#endif //USER_HPP


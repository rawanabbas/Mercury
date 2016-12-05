#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "stdafx.h"
#include "user.hpp"
#include "database.hpp"

enum class DatabaseStatus {
    Initializing,
    Ready,
    Error
};

enum class DatabaseOperation {
    Select,
    Insert,
    Delete,
    Update,
    Create
};

class Database {

    static const std::string UsernameColumnToken;
    static const std::string UserIDColumnToken;
    static const std::string PasswordColumnToken;

    sqlite3 * _db;
    DatabaseStatus _status;
    std::string _name;

    bool _create();

    User _fetch(std::string username);
    std::vector<Peer> _fetch();

    bool _insert(std::string username, std::string password, std::string userID);
    bool _remove(std::string username);

    static int _fetchCallback(void *userPtr, int argc, char **argv, char **column);
    static int _fetchAllCallback(void *userPtr, int argc, char **argv, char **column);

public:
    Database(std::string name = "database.db");

    bool insert(User user);
    User fetch(User user);
    User fetch(std::string username);
    std::vector<Peer> fetch();
    bool remove(User user);
    bool remove(std::string username);

    DatabaseStatus getStatus();
    std::string getName();

    virtual ~Database();

};

#endif // DATABASE_HPP

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "stdfax.h"
#include "user.hpp"

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

//    bool _exec(DatabaseOperation op, int (*callback)(void*,int,char**,char**) = NULL, User user, std::string data);

    bool _create();
    User _fetch(std::string username);
    bool _insert(std::string username, std::string password, std::string userID);
    bool _remove(std::string username);

    static int _fetchCallback(void *data, int argc, char **argv, char **column);

public:
    Database(std::string name = "database.db");

    bool insert(User user);
    User fetch(User user);
    User fetch(std::string username);
    bool remove(User user);
    bool remove(std::string username);

    DatabaseStatus getStatus();
    std::string getName();

    virtual ~Database();

};

#endif // DATABASE_HPP

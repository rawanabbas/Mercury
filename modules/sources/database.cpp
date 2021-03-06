#include "database.hpp"

const std::string Database::UsernameColumnToken = "username";
const std::string Database::UserIDColumnToken = "userID";
const std::string Database::PasswordColumnToken = "password";

Database::Database(std::string name) : _name(name) {

    if (sqlite3_open_v2(_name.c_str(), &_db, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {

        _status = DatabaseStatus::Error;
        std::cerr << "Can't open database: " << sqlite3_errmsg(_db) << std::endl;
        sqlite3_close(_db);

    } else {

        _status = DatabaseStatus::Initializing;
        _create();
    }

}

bool Database::_create() {

    char *error;
    std::string sql = "CREATE TABLE IF NOT EXISTs Users(id INTEGER PRIMARY KEY, username TEXT NOT NULL, password TEXT NOT NULL, userID TEXT NOT NULL, CONSTRAINT username_constraint UNIQUE(username));";

    if(sqlite3_exec(_db, sql.c_str(), NULL, 0, &error) != SQLITE_OK) {

        _status = DatabaseStatus::Error;
        std::cerr << "An error has occured while creating the USER table: " << error << std::endl;
        return false;

    } else {

        _status = DatabaseStatus::Ready;
        return true;

    }
}

User Database::_fetch(std::string username) {

    char *error;
    std::string sql = "SELECT * FROM USERS WHERE username = '" + username + "';";

    User user;

    if (sqlite3_exec(_db, sql.c_str(), _fetchCallback, (void *)&user, &error) != SQLITE_OK) {

        std::cerr << "An error has occured while selecting the user: " << error << std::endl;
        throw "Selecting User Failed!";

    } else {

        return user;

    }

}

std::vector<Peer> Database::_fetch() {

    char *error;
    std::string sql = "SELECT * FROM USERS;";

    std::vector<Peer> peers;

    if (sqlite3_exec(_db, sql.c_str(), _fetchAllCallback, (void *)&peers, &error) != SQLITE_OK) {

        std::cerr << "An error has occured while selecting the user: " << error << std::endl;
        throw "Selecting All Users Failed!";

    } else {

        return peers;

    }

}

bool Database::_insert(std::string username, std::string password, std::string userID) {

    char *error;

    std::string sql = "INSERT INTO Users(username, password, userID) VALUES('"
                      + username + "', '" + password + "', '" + userID + "');";

    if (sqlite3_exec(_db, sql.c_str(), NULL, 0, &error) != SQLITE_OK) {

        std::cerr << "An error has occured while inserting the user: " << error << std::endl;
        return false;

    } else {

        return true;

    }
}

bool Database::_remove(std::string username) {
    char *error;
    std::string sql = "DELETE FROM Users WHERE username = " + username + ";";

    if (sqlite3_exec(_db, sql.c_str(), NULL, 0 , &error) != SQLITE_OK) {

        std::cerr <<"An error has occured while deleting the user: " << error << std::endl;
        return false;

    } else {

        return true;

    }

}

int Database::_fetchCallback(void *userPtr, int argc, char **argv, char **column) {

    User* user = (User*) userPtr;

    for (int i = 0; i < argc; ++i) {
        if (std::string(column[i]) == UsernameColumnToken) {

            user->setUsername(argv[i]);
            continue;

        } else if (std::string(column[i]) == UserIDColumnToken) {
            user->setUserID(argv[i]);
            continue;

        } else if (std::string(column[i]) == PasswordColumnToken) {

            user->setPasswordWHash(argv[i]);
            continue;

        }


    }

    return 0;

}

int Database::_fetchAllCallback(void *userPtr, int argc, char **argv, char **column) {

    std::vector<Peer> * peers = (std::vector<Peer> *) userPtr;

    Peer peer;

    for (int i = 0; i < argc; ++i) {
        if (std::string(column[i]) == UsernameColumnToken) {

            peer.setUsername(argv[i]);
            continue;

        } else if (std::string(column[i]) == UserIDColumnToken) {
            peer.setUserID(argv[i]);
            continue;

        }

    }

    peers->push_back(peer);

    return 0;


}

bool Database::insert(User user) {

    std::cout << "Database Status: " << (int) getStatus() << std::endl;
    if (getStatus() == DatabaseStatus::Ready) {

        return _insert(user.getUsername(), user.getPassword(), user.getUserID());

    } else {

        std::cerr << "Database is not ready!" << std::endl;
        return false;

    }
}

User Database::fetch(User user) {

    return fetch(user.getUsername());

}

User Database::fetch(std::string username) {

    if (getStatus() == DatabaseStatus::Ready) {

        return _fetch(username);

    } else {

        throw "An error has occured while fetching the user!";

    }

}

std::vector<Peer> Database::fetch() {

    if (getStatus() == DatabaseStatus::Ready) {

        return _fetch();

    } else {

        throw "An error has occured while fetching the user!";

    }
}

bool Database::remove(User user) {

    return remove(user.getUsername());

}

bool Database::remove(std::string username) {

    if (getStatus() == DatabaseStatus::Ready) {

        return _remove(username);

    } else {

        std::cerr << "Database is not ready!" << std::endl;
        return false;
    }

}


DatabaseStatus Database::getStatus() {
    return _status;
}

std::string Database::getName() {
    return _name;
}

Database::~Database() {

    sqlite3_close(_db);

}

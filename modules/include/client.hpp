/**
 * class Client
 * Description: A class for encuaplsating and abstracting the client object
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "stdfax.h"
#include "udp_socket.hpp"
#include "thread.hpp"
#include "message.hpp"
#include "file.hpp"

enum class Commands : char {
    Exit = 'q',
    File = 'f',
    SendFile = 's',
    ReadFile = 'r',
    WriteFile = 'w',
    OpenFile = 'o',
    CreateFile = 'c',
    Text = 't',
    Register = 'r',
    SignIn = 'a',
    Ping = 'p'
};

enum class ClientAuthenticationStatus {
    Authenticating,
    Authenticated,
    Unauthenticated
};

class Client : public Thread {

private:
    static int _id;

    std::string _command;
    bool _isAuthenticated;
    ClientAuthenticationStatus _authStatus;

    pthread_mutex_t _commandMutex;

    void _execute();

    bool _createFile();
    bool _createFile(File *remoteFile);
    bool _createFile(File *remoteFile, std::string fileName);

    void _readFile();
    void _sendFile();
    void _writeFile();
    void _openFile();
    void _handleFile(std::string msg);

protected:
    UDPSocket _sock;
    UDPSocket _serverSocket;

    char _buffer[MAX_RECV];
    std::string _ownerId;

    std::map<FileDescriptor, File*> _files;

    void _updateServerSocket(int port, std::string host);

    bool _sendMessage(Message message);
    bool _send(std::string msg);

    bool _receive(std::string &msg);
    int _receiveWithTimeout(std::string &msg, int timeout = 2);

    void _exit(std::string msg);
    void _ping(std::string msg = "Ping!");

    void _register();
    void _authenticate();

    bool _lock();
    bool _unlock();

    void _clearCommand();

public:

    Client (std::string ownerId, std::string hostname, int serverPort);
    Client (std::string hostname, int serverPort);
    virtual ~Client ();

    void setCommand(std::string command);
    bool isAuthenticated();

    void run();

    ClientAuthenticationStatus getAuthStatus();

    std::string getOwnerId() const;
    void setOwnerId(const std::string &ownerId);
};

#endif // CLIENT_HPP

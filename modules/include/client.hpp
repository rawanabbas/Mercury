/**
 * class Client
 * Description: A class for encuaplsating and abstracting the client object
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "stdafx.h"
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
    Register = 'g',
    SignIn = 'a',
    Ping = 'p',
    EstablishConnection = 'e'
};

enum class ClientStatus {
    Pinging,
    EstablishConnection,
    Sending,
    Recieving,
    Authenticating,
    Authenticated,
    Unauthenticated,
    Waiting,
    Failed
};

class Client : public Thread {

private:
    static int _id;

    ClientStatus _status;

    std::string _command;

    std::vector<std::string> _argv;
    int _argc;

    bool _isAuthenticated;

    pthread_mutex_t _commandMutex;

    void _execute();

    void _handleFile();

    bool _createFile();
    bool _createFile(File *remoteFile);
    bool _createFile(File *remoteFile, std::string fileName);


    void _readFile();
    void _sendFile();
    void _writeFile();
    void _openFile();

protected:

    std::string _host;
    std::string _username;

    int _serverPort;

    UDPSocket _sock;
    UDPSocket _serverSocket;
    Callback _callback;

    char _buffer[MAX_RECV];

    std::string _ownerId;

    std::map<FileDescriptor, File*> _files;

    void _updateServerSocket(int port, std::string host);

    bool _sendMessage(Message message);
    bool _send(std::string msg);

    bool _receive(std::string &msg);
    int _receiveWithTimeout(std::string &msg, int timeout = 2);

    void _exit(std::string msg);
    void _ping();

    void _register();
    void _authenticate();

    int _lock();
    int _unlock();

    void _clearCommand();

public:

    Client (std::string ownerId, std::string username, std::string hostname, int serverPort);
    Client (std::string hostname, int serverPort);

    void setCommand(std::string command, Callback callback);

    std::vector<std::string> getArguments() const;
    void setArguments(const std::vector<std::string> &args);
    void addArgument(std::string arg);

    bool isAuthenticated();


    ClientStatus getStatus();

    void run();


    std::string getOwnerId() const;
    void setOwnerId(const std::string &ownerId);

    std::string getHost() const;

    int getServerPort() const;

    virtual ~Client ();
    std::string getUsername() const;
    void setUsername(const std::string &username);
};

#endif // CLIENT_HPP

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
    Ping = 'p'
};

class Client : public Thread {

private:
    static int _id;
    void _execute();
    bool _createFile();
    bool _createFile(File *remoteFile);
    bool _createFile(File *remoteFile, std::string fileName);
    void _readFile();
    void _sendFile();
    void _writeFile();
    void _handleFile(std::string msg);
    void _openFile();

protected:
    UDPSocket _sock;
    UDPSocket _serverSocket;
    char _buffer[MAX_RECV];
    std::map<FileDescriptor, File*> _files;

    void _updateServerSocket(int port, std::string host);
    bool _sendMessage(Message message);
    bool _send(std::string msg);
    bool _receive(std::string &msg);
    int _receiveWithTimeout(std::string &msg);
    void _exit(std::string msg);
    void _ping(std::string msg = "Ping!");
public:
    Client (std::string hostname, int serverPort, int port = 0);
    virtual ~Client ();

    void run();
};

#endif // CLIENT_HPP

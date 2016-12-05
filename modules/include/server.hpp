/**
* class Server
* Description: A class for encuaplsating and abstracting the server object, it inherits from thread
* Author: Group 3
* Date: 24th October 2016
*  Project One - CSCE 4411
*/
#ifndef SERVER_HPP
#define  SERVER_HPP

#include "stdafx.h"
#include "udp_socket.hpp"
#include "thread.hpp"
#include "job.hpp"
#include "message.hpp"
#include "file.hpp"


class Server : public Thread {
private:

    UDPSocket _sock;
    std::string _ownerId;
    std::string _username;

    char _buffer[MAX_RECV];

    std::vector<UDPSocket*> _clients;
    std::vector<Job*> _jobs;

    FilesMap _files;
    pthread_mutex_t _filesMutex;

    void _terminateJob(Job *job);
    static void * _callbackWrapper(Thread * thread, void * parent);
    void _spawnJob(UDPSocket clientSocket);

public:

    Server (std::string ownerId, std::string username, int port = 0);

    void listen();
    void accept(UDPSocket& client);

    void run ();
    int getServerPort();

    void writeBuffer(char * msg);
    char * readBuffer(int start = 0, int end = MAX_RECV);

    void addFileRecepient(std::string username, std::string filename);

    virtual ~Server ();
};

#endif //SERVER_HPP

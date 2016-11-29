/**
 * class Job
 * A class for encuaplsating and abstracting the job which is a worker for the parent thread.
 * It inherts from the thread class
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */
#ifndef JOB_HPP
#define JOB_HPP

#include "stdfax.h"
#include "udp_socket.hpp"
#include "thread.hpp"
#include "message.hpp"
#include "file.hpp"


enum class JobState {
    Running,
    Exit
};

class Job : public Thread {
private:
    void *_parent;
    static int _id;
    std::string _ownerId;

    UDPSocket _clientSocket;
    UDPSocket _serverSocket;

    Message _msg;

    std::map<FileDescriptor, File*> _files;

    //Private Function Handlers

    bool _sendInfo();
    void _listen();
    void _openFile(Message message);
    void _createFile(Message &message);
    void _readFile(Message &message);
    void _writeFile(Message &message);
    JobState _handleMessage(Message message);

public:

    Job (std::string ownerId, UDPSocket sock);
    int getJobId() const;
    virtual ~Job ();
    void run();

    inline bool operator == (const Job& job);
};

#endif

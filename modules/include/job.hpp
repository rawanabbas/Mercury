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

#include "stdafx.h"
#include "udp_socket.hpp"
#include "thread.hpp"
#include "message.hpp"
#include "file.hpp"
#include "client.hpp"

typedef std::map<std::string, std::vector<std::string> > FilesMap;
typedef std::map<FileDescriptor, File* > FileDescriptorMap;

enum class JobState {
    Running,
    Exit
};

class Job : public Thread {
private:
    void *_parent;
    static int _id;
    std::string _ownerId;
    std::string _username;

    UDPSocket _clientSocket;
    UDPSocket _serverSocket;

    FilesMap * _fileRecipients;
    pthread_mutex_t _filesMutex;

    Message _msg;

    FileDescriptorMap _files;

    //Private Function Handlers

    bool _sendInfo();
    void _listen();

    void _openFile(Message message);
    void _createFile(Message &message);
    void _readFile(Message &message);
    void _writeFile(Message &message);

    bool _createRemoteFile(File *remoteFile, std::string fileName);

    JobState _handleMessage(Message message);

public:

    Job (std::string ownerId, std::string username, UDPSocket sock, FilesMap *files, pthread_mutex_t filesMutex);
    int getJobId() const;
    virtual ~Job ();
    void run();

    inline bool operator == (const Job& job);
};

#endif

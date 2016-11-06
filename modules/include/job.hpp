#ifndef JOB_HPP
#define JOB_HPP

#include "stdfax.h"
#include "udp_socket.hpp"
#include "thread.hpp"

class Job : public Thread {
private:
    static int _id;
    UDPSocket _clientSocket;
    UDPSocket _serverSocket;
    void *_parent;
public:
    Job (UDPSocket sock);
    int getJobId() const;
    virtual ~Job ();
    void run();

    inline bool operator == (const Job& job);
};

#endif
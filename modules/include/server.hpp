#ifndef SERVER_HPP
#define  SERVER_HPP

#include "stdfax.h"
#include "udp_socket.hpp"
#include "thread.hpp"
#include "job.hpp"

class Server : public Thread {
private:
    UDPSocket _sock;
    std::vector<UDPSocket*> _clients;
    std::vector<Job*> _jobs;
    char _buffer[MAX_RECV];

    void _terminateJob(Job *job);
    static void * _callbackWrapper(Thread * thread, void * parent);

public:
    Server (int port = 0);
    virtual ~Server ();
    void listen();
    void accept(UDPSocket& client);

    void run ();
    int getServerPort();

    void writeBuffer(char * msg);
    char * readBuffer(int start = 0, int end = MAX_RECV);
};

#endif //SERVER_HPP

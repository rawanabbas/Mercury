#ifndef HEARTBEAT_HPP
#define HEARTBEAT_HPP

#include "stdafx.h"
#include "udp_socket.hpp"
#include "thread.hpp"
#include "client.hpp"
#include "message.hpp"

enum class Status {
    EstablishingConnection,
    Ping,
    Waiting,
    Failed,
    Pong
};

class Heartbeat : public Client {

    pthread_condattr_t _timerAttr;
    pthread_mutex_t _timerMutex;
    pthread_cond_t _timerConditional;

    Status _status;


protected:

    timespec _pingTime;
    int _retry;

    bool _establishConnection();

    void _wait(long pingTime);
    void _resetTrials();


public:

    Heartbeat(std::string ownerId, std::string host, int port);

    Status getStatus();

    void run();

    virtual ~Heartbeat();


};

#endif //HEARTBEAT_HPP

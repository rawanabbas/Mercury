/**
 * class Thread
 * Description: A class for encuaplsating and abstracting the thread object using the linux library pthread
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */
#ifndef THREAD_HPP
#define THREAD_HPP

#include "stdafx.h"

class Thread;

typedef void* (*ThreadCallback)(Thread *, void*);

class Thread {

private:
    pthread_t _thread;
    bool _isPending;

    pthread_mutex_t _mutex;

    ThreadCallback _doneCallback;
    static void* _run(void *);

protected:

    bool _isRunning;
    void * _parent;

public:

    Thread ();

    virtual ~Thread ();
    virtual void run () = 0;

    void start();
    void join();

    void lock(pthread_mutex_t mutex);
    void release(pthread_mutex_t mutex);

    int lock();
    int release();

    void exit();

    pthread_mutex_t getMutex() const;
    void setMutex(pthread_mutex_t mutex);

    void setDoneCallback(ThreadCallback, void * parent = NULL);
    void setParent(void *parent);

    long getThreadId ();

    bool isRunning();
    bool isPending();
};

#endif //THREAD_HPP

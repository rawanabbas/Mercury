/**
 * class Thread
 * Description: A class for encuaplsating and abstracting the thread object using the linux library pthread
 * Author: Group 3
 * Date: 24th October 2016
 *  Project One - CSCE 4411
 */
#include "thread.hpp"

Thread::Thread() : _isRunning(false), _isPending(true), _doneCallback(NULL), _mutex(nullptr) {

}

Thread::~Thread() {
}

void Thread::start() {
    pthread_create(&_thread, NULL, _run, (void *) this);
}


pthread_mutex_t *Thread::getMutex() const {
    return _mutex;
}

void Thread::setMutex(pthread_mutex_t *mutex) {
    std::cout << "setMutex " << (mutex == nullptr) << std::endl;
    _mutex = mutex;
}

void * Thread::_run(void *thisThread) {

    Thread* threadPtr = (Thread*) thisThread;

    threadPtr->run();

    if (threadPtr->_doneCallback != NULL) {

        (*(threadPtr->_doneCallback))(threadPtr, threadPtr -> _parent);

    }

    pthread_exit(0);
}

long Thread::getThreadId() {
    return (long long) pthread_self();
}

void Thread::join() {
    pthread_join(_thread, NULL);
}

void Thread::exit() {
    int returnValue = SUCCESS;
    pthread_exit(&returnValue);
}

bool Thread::isPending() {
    return _isPending;
}

bool Thread::isRunning() {
    return _isRunning;
}

int Thread::lock() {
    std::cout << "lock" << std::endl;
    if (_mutex == nullptr) {
        std::cout << "!!!!!!!" << std::endl;
        throw "Locking null mutex";
    }
    return pthread_mutex_lock(_mutex);
}

int Thread::release() {
    return pthread_mutex_unlock(_mutex);
}

void Thread::lock(pthread_mutex_t mutex) {
    pthread_mutex_lock(&mutex);
}

void Thread::release(pthread_mutex_t mutex) {
    pthread_mutex_unlock(&mutex);
}

void Thread::setDoneCallback(ThreadCallback callback, void *parent) {

    _doneCallback = callback;
    _parent = parent;

}

void Thread::setParent(void *parent) {
    _parent = parent;
}

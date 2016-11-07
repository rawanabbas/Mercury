#include "thread.hpp"

Thread::Thread() : _isRunning(false), _isPending(true), _doneCallback(NULL) {

}

Thread::~Thread() {
}

void Thread::start() {
    pthread_create(&_thread, NULL, _run, (void *) this);
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

void Thread::lock() {
    pthread_mutex_lock(&_mutex);
}

void Thread::release() {
    pthread_mutex_unlock(&_mutex);
}

void Thread::setDoneCallback(ThreadCallback callback, void *parent) {
    _doneCallback = callback;
    _parent = parent;
}

void Thread::setParent(void *parent) {
    _parent = parent;
}

#include "thread.h"


bool Thread::running() const
{
    return _running;
}

void Thread::setRunning(bool running)
{
    _running = running;
}

bool Thread::constructed() const
{
    return _constructed;
}

void Thread::setConstructed(bool constructed)
{
    _constructed = constructed;
}

bool Thread::finished() const
{
    return _finished;
}

void Thread::setFinished(bool finished)
{
    _finished = finished;
}
Thread::Thread()
{
}

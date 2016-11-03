#ifndef THREAD_H
#define THREAD_H

class Thread
{
private:
    bool _running;
    bool _constructed;
    bool _finished;
public:
    Thread();
    virtual static void run();
    bool isRunning() const;
    bool isConstructed() const;
    bool isFinished() const;
};

#endif // THREAD_H

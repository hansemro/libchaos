/*****************************************
**               LibChaos               **
**               zthread.h              **
**       (c) 2013 Zennix Studios        **
*****************************************/
#ifndef ZTHREAD_H
#define ZTHREAD_H

#include "ztypes.h"

#if COMPILER != MSVC
    #include <pthread.h>
#endif

#include <atomic>

#undef bool // WTF ?

namespace LibChaos {

typedef unsigned long ztid;

struct ZThreadArg {
    bool stop();
    void *arg;
    std::atomic<bool> *_stop;
};

class ZThread {
public:
    typedef void *(*funcType)(void *);

public:
    ZThread();
    ZThread(funcType);
    ZThread(funcType, void *);
    ZThread(const ZThread &other);

    ~ZThread();

    bool run(funcType fnc);
    bool run(funcType fnc, void *arg);
    void *join();
    void kill();
    void stop();
    void detach();

    static void yield();

    static void *entry(void *ptr);

    void setCopyable();

    ztid tid();
    bool alive();
    static ztid thisTid();

private:
    struct zthreadparam {
        funcType funcptr;
        ZThreadArg zarg;
    };

    typedef void * HANDLE;

private:
    zthreadparam _param;
    std::atomic<bool> _stop;
    int ret;
#if COMPILER == MSVC
    HANDLE _thread;
#else
    pthread_t _thread;
#endif
    bool _alive;
    bool copyable;
};

typedef ZThread ZThreadA;

} // namespace LibChaos

#endif // ZTHREAD_H

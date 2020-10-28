#ifndef CEQUTHREAD_H
#define CEQUTHREAD_H

//#include"public.h"


#include <pthread.h>
#include <iostream>
#include <signal.h>

using namespace std;


class ThreadBase
{
private:
    pthread_t m_tid;
    bool      m_isAlive;

private:
    static void* start_func(void* arg);

public:
    int start();
    int join();
    int quit();

    pthread_t getTid();
    bool isAlive();

    virtual void run() = 0;
    virtual ~ThreadBase();
};


#endif // CEQUTHREAD_H

// sl_lock.cc - Implementation of lock seaming operation class, 
//      use CRITICAL_SECTION under Windows and pthread_mutex_t 
//      under Linux
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/10 10:55:28 by ASMlover.
#if _WIN32 || _WIN64
    #include <windows.h>
#else 
    #include <pthread.h>
#endif

#include "../inc/sl_lock.h"


namespace sl {

#if _WIN32 || _WIN64
    #define LOCKER          CRITICAL_SECTION
    #define LOCKER_NEW(x)   InitializeCriticalSection((x))
    #define LOCKER_FREE(x)  DeleteCriticalSection((x))
    #define LOCKER_ON(x)    EnterCriticalSection((x))
    #define LOCKER_OFF(x)   LeaveCriticalSection((x))
#else
    #define LOCKER          pthread_mutex_t
    #define LOCKER_NEW(x)   pthread_mutex_init((x), NULL)
    #define LOCKER_FREE(x)  pthread_mutex_destroy((x))
    #define LOCKER_ON(x)    pthread_mutex_lock((x))
    #define LOCKER_OFF(x)   pthread_mutex_unlock((x))
#endif


    Lock::Lock(void)
    {
        locker_ = new LOCKER;

        if (NULL != locker_)
            LOCKER_NEW((LOCKER*)locker_);
    }

    Lock::~Lock(void)
    {
        if (NULL != locker_)
        {
            LOCKER_FREE((LOCKER*)locker_);
            delete (LOCKER*)locker_;
            locker_ = NULL;
        }
    }

    void Lock::lock(void)
    {
        if (NULL != locker_)
            LOCKER_ON((LOCKER*)locker_);
    }

    void Lock::unlock(void)
    {
        if (NULL != locker_)
            LOCKER_OFF((LOCKER*)locker_);
    }

}
// sl_lock.h - Lock seaming operation class, use CRITICAL_SECTION
//      under Windows and pthread_mutex_t under Linux
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/10 10:51:46 by ASMlover.
#ifndef __SL_LOCK_HEADER_H__
#define __SL_LOCK_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

namespace sl {

    class Lock 
    {
        void* locker_;  // the object of locker
    public:
        Lock(void);
        ~Lock(void);
    public:
        void lock(void);
        void unlock(void);
    };

}

#endif  // __SL_LOCK_HEADER_H__
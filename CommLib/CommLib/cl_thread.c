/*
 * Copyright (c) 2010, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 *
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#if _WIN32 || _WIN64
#include <windows.h>
#if _MT
#include <process.h>
#endif
#else
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#endif
#include "cl_thread.h"


#if _WIN32 || _WIN64

#if _MT
typedef unsigned (__stdcall *_ThreadProc)(void*);
#define __Thread_New(_t_obj, _func, _args)\
    _t_obj = (Thread_Object)_beginthreadex(0, 0, (_ThreadProc)_func, _args, 0, 0)
#else
typedef unsigned long (__stdcall *_ThreadProc)(void*);
#define __Thread_New(_t_obj, _func, _args)\
    _t_obj = (Thread_Object)CreateThread(0, 0, (_ThreadProc)_func, _args, 0, 0)
#endif 
#define __Thread_Free(_t_obj) {\
    if (0 != _t_obj) { CloseHandle(_t_obj); _t_obj = 0; }\
}

#define __Event_New(_e_obj)     _e_obj = CreateEvent(0, TRUE, FALSE, 0)
#define __Event_Free(_e_obj) {\
    if (0 != _e_obj) { CloseHandle(_e_obj); _e_obj = 0; }\
}
#define __Event_Set(_e_obj)     SetEvent(_e_obj)

#define __Thread_Stop(_e_obj, _time)\
    (WAIT_OBJECT_0 == WaitForSingleObject(_e_obj, _time))
#define __Thread_Wait_Timeout(_t_obj)\
    (WAIT_TIMEOUT == WaitForSingleObject(_t_obj, 100))
#define __Thread_Kill(_t_obj)   TerminateThread(_t_obj, ~0)

#define __sleep(_millitm)       Sleep(_millitm)

#else

#define __Thread_New(_t_obj, _func, _args)\
    pthread_create(&_t_obj, 0, _func, _args)
#define __Thread_Free(_t_obj)

#define __Event_New(_e_obj)\
    pthread_cond_init((pthread_cond_t*)_e_obj, 0)
#define __Event_Free(_e_obj) {\
    if (0 != _e_obj) { pthread_cond_destroy((pthread_cond_t*)&_e_obj); _e_obj = 0; }\
}
#define __Event_Set(_e_obj)     pthread_cond_signal((pthread_cond_t*)&_e_obj)

#define __Thread_Stop(_e_obj, _time)\
    (ETIMEDOUT != pthread_cond_timedwait((pthread_cond_t*)&_e_obj, 0, &_time))
#define __Thread_Wait_Timeout(_t_obj)\
    (0 == pthread_join((pthread_t)_t_obj, 0))
#define __Thread_Kill(_t_obj)

#define __sleep(_millitm)       usleep(_millitm * 1E3)

#endif


int safethread_init(struct _Thread* tp, _thread_func func, void* args) {
    if (0 == tp || 0 == func)
        return -1;
    __Event_New(tp->_e_obj);
    __Thread_New(tp->_t_obj, func, args);
    __sleep(100);
    return (0 != tp->_e_obj && 0 != tp->_t_obj ? 0 : -1);
}

int safethread_stop(struct _Thread t) {
#if _WIN32 || _WIN64
    return __Thread_Stop(t._e_obj, 5);
#else
    struct timeval  now = {0};
    struct timespec _tm = {0};
    gettimeofday(&now, 0);
    _tm.tv_nsec = 5 * 1E6;
    _tm.tv_sec  = time(0); 
    return __Thread_Stop(t._e_obj, _tm);
#endif
}

void safethread_uninit(struct _Thread* tp) {
    if (0 != tp) {
        __Event_Set(tp->_e_obj);
        if (__Thread_Wait_Timeout(tp->_t_obj))
            __Thread_Kill(tp->_t_obj);
        __Thread_Free(tp->_t_obj);
        __Event_Free(tp->_e_obj);
    }
}

Thread_Object thread_new(_thread_func func, void* args) {
    Thread_Object t_obj = 0;

    if (0 != func)
        __Thread_New(t_obj, func, args);
    return t_obj;
}

void thread_free(Thread_Object* t_obj) {
    __Thread_Free(*t_obj);
}
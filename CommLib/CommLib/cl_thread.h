/*
 * Copyright (c) 2010, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 *
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#ifndef __CL_THREAD_HEADER_H__
#define __CL_THREAD_HEADER_H__

#if _WIN32 || _WIN64
typedef unsigned (*_thread_func)(void*);
#else
typedef void* (*_thread_func)(void*);
#endif
typedef void* Thread_Object;
typedef void* Event_Object;

struct _Thread {
    Thread_Object _t_obj;
    Event_Object  _e_obj;
};

extern int safethread_init(struct _Thread* tp, _thread_func func, void* args);
extern int safethread_stop(struct _Thread t);
extern void safethread_uninit(struct _Thread* tp);

extern Thread_Object thread_new(_thread_func func, void* args);
extern void thread_free(Thread_Object* t_obj);

#endif  /* __CL_THREAD_HEADER_H__ */
// sl_network_test_lock.cc - testing class sl::Lock
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/10 11:08:26 by ASMlover.
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "../../inc/sl_lock.h"

#include "sl_network_test.h"



namespace sl_test {

    struct LockThreadArgs 
    {
        HANDLE   hThread_;
        HANDLE   hEvent_;
        sl::Lock l_;
        std::vector<int> dat_;

        LockThreadArgs(void)
            : hThread_(NULL), hEvent_(NULL) 
        {
            dat_.clear();
        }
    };

    static DWORD WINAPI 
    __sInsertThread(void* args_)
    {
        LockThreadArgs* p = (LockThreadArgs*)args_;

        if (NULL == p)
            return -1L;
        srand((uint)time(0));
        while (true) 
        {
            if (WAIT_OBJECT_0 == WaitForSingleObject(p->hEvent_, 5))
                break;

            p->l_.lock();
            p->dat_.push_back(rand() % 10000);
            p->l_.unlock();
        }
        fprintf(stdout, "__sInsertThread quit safety ... \n");
        return 0L;
    }

    void 
    sl_network_lock(void)
    {
        LockThreadArgs ta;
        int        count = 0;
        std::vector<int>::iterator i;

        ta.hEvent_  = CreateEvent(NULL, TRUE, FALSE, NULL);
        ta.hThread_ = CreateThread(NULL, 0, __sInsertThread, &ta, 0L, NULL);

        while (count++ < 1000)
        {
            ta.l_.lock();
            fprintf(stdout, "NOW data size is : [%d]\n", ta.dat_.size());

            for (i = ta.dat_.begin(); i != ta.dat_.end(); ++i)
                fprintf(stdout, "%d\t", *i);
            fprintf(stdout, "\n\n");
            ta.l_.unlock();

            Sleep(5);
        }
        SetEvent(ta.hEvent_);
        WaitForSingleObject(ta.hThread_, INFINITE);
        SAFEHANDLE_FREE(ta.hThread_);
        SAFEHANDLE_FREE(ta.hEvent_);
        ta.dat_.clear();
    }

}
// sl_network_test_buffer.cc - testing class sl::Buffer
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/11 17:11:55 by ASMlover.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../inc/sl_buffer.h"
#include "sl_network_test.h"


namespace sl_test {

#define COUNT   (100)

    struct BufferTransArgs
    {
        sl::Buffer b_;
        HANDLE hThread_;
        HANDLE hEvent_;
    };

    static void 
    __showShortData(const short* dat_, int sz_)
    {
        int i = 0, count = sz_ / sizeof(short);

        if (NULL != dat_)
        {
            for (i = 0; i < count; ++i)
                fprintf(stdout, "[%.3d] = %d\t", i + 1, dat_[i]);
            fprintf(stdout, "\n\n\n");
        }
    }

    static DWORD WINAPI 
    __sReadThread(void* args_)
    {
        BufferTransArgs* p = (BufferTransArgs*)args_;
        short  buf[COUNT] = {0};
        int    i = 0;
        size_t readLen = 0;

        if (NULL == p)
            return -1;
        Sleep(100);
        while (true) 
        {
            if (WAIT_OBJECT_0 == WaitForSingleObject(p->hEvent_, 5))
                break;
            memset(buf, 0, sizeof(buf));
            readLen = p->b_.read(sizeof(buf), (byte*)buf);
            if (readLen == sizeof(buf))
                __showShortData(buf, readLen);
        }

        return 0;
    }

    void 
    sl_network_buffer(void)
    {
        int i = 0, count = 0;
        short dat[COUNT] = {0};

        BufferTransArgs ta;

        ta.b_.create();

        ta.hEvent_  = CreateEvent(NULL, TRUE, FALSE, NULL);
        ta.hThread_ = CreateThread(NULL, 0, __sReadThread, &ta, 0L, NULL);

        while (true)
        {
            srand((uint)time(0));
            for (i = 0; i < COUNT; ++i)
                dat[i] = rand() % 100;
            ta.b_.write((byte*)dat, sizeof(dat));

            srand((uint)GetTickCount());
            if (5 == rand() % 100) 
            {
                fprintf(stdout, "\n\n *************** clear buffer space ******************\n\n\n");
                ta.b_.clear();
                Sleep(1000);
            }

            Sleep(5);
        }
        SetEvent(ta.hEvent_);
        WaitForSingleObject(ta.hThread_, INFINITE);
        SAFEHANDLE_FREE(ta.hThread_);
        SAFEHANDLE_FREE(ta.hEvent_);

        ta.b_.release();
    }

}
// sl_network_test_frame.cc - Testing FrameBlock class 
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/13 16:59:14 by ASMlover.
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include "../../inc/sl_frame.h"

#include "sl_network_test.h"


namespace sl_test {

    struct FrameThreadArgs 
    {
        sl::FrameBlock   fb_;
        CRITICAL_SECTION cs_;

        HANDLE hThread_;
        HANDLE hEvent_;
    };

    int 
    __frame_show_data(const short* dat, size_t sz)
    {
        int i = 0, count = sz / sizeof(short);

        if (NULL != dat && sz > 0) 
        {
            for (i = 0; i < count; ++i)
                fprintf(stdout, "[%.3d] = %d\t", i + 1, dat[i]);
            fprintf(stdout, "\n\n");

            return 0;
        }
        return -1;
    }

    DWORD WINAPI 
    __frameReadThread(void* args)
    {
        FrameThreadArgs* fta = (FrameThreadArgs*)args;

        if (NULL == fta)
            return -1L;
        while (true) 
        {
            if (WAIT_OBJECT_0 == WaitForSingleObject(fta->hEvent_, 100))
                break;
            EnterCriticalSection(&fta->cs_);
            __frame_show_data((short*)fta->fb_.get(), fta->fb_.size());
            LeaveCriticalSection(&fta->cs_);
        }

        return 0L;
    }

    void 
    sl_network_frame(void)
    {
        int   i = 0, count = 0;
        short data[200] = {0};
        FrameThreadArgs fta;

        InitializeCriticalSection(&fta.cs_);

        fta.hEvent_  = CreateEvent(NULL, TRUE, FALSE, NULL);
        fta.hThread_ = CreateThread(NULL, 0, __frameReadThread, &fta, 0L, NULL);
        while (count++ < 1000) 
        {
            srand((uint)time(0));
            for (i = 0; i < 200; ++i)
                data[i] = rand() % 100;

            EnterCriticalSection(&fta.cs_);
            fta.fb_.allocBlock((byte*)data, sizeof(data));
            LeaveCriticalSection(&fta.cs_);

            Sleep(5);
        }

        SetEvent(fta.hEvent_);
        WaitForSingleObject(fta.hThread_, INFINITE);
        SAFEHANDLE_FREE(fta.hThread_);
        SAFEHANDLE_FREE(fta.hEvent_);

        fta.fb_.freeBlock();

        DeleteCriticalSection(&fta.cs_);
    }

}
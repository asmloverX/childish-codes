// sl_network_test_circularbuffer.cc - Testing class sl::CircularBuffer
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/11 18:53:35 by ASMlover.
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include "../../inc/sl_circularbuffer.h"
#include "../../inc/sl_frame.h"

#include "sl_network_test.h"



namespace sl_test {

    struct CB_ARGS
    {
        sl::CircularBuffer cb_;
        sl::FrameBlock     fb_;
        CRITICAL_SECTION   cs_;

        CB_ARGS(void)
        {
            InitializeCriticalSection(&cs_);
        }

        ~CB_ARGS(void)
        {
            DeleteCriticalSection(&cs_);
        }

        void lock(void)
        {
            EnterCriticalSection(&cs_);
        }

        void unlock(void)
        {
            LeaveCriticalSection(&cs_);
        }
    };

    void 
    __cb_show_data(const short* dat, size_t sz)
    {
        int i = 0, count = sz / sizeof(short);

        if (NULL != dat && sz > 0) {
            for (i = 0; i < count; ++i)
                fprintf(stdout, "[%.3d] = %d\t", i + 1, dat[i]);
            fprintf(stdout, 
                "\n***************** data count is %d ****************\n\n\n\n\n\n", count);
        }
    }

    DWORD WINAPI 
    __cbShowThread(void* args)
    {
        CB_ARGS* ca = (CB_ARGS*)args;

        if (NULL == ca)
            return -1;
        while (true)
        {
            ca->lock();
            if (ca->fb_.isValid())
                __cb_show_data((short*)ca->fb_.get(), ca->fb_.size());
            ca->unlock();
            Sleep(10);
        }
    
        return 0;
    }

    DWORD WINAPI 
    __cbReadThread(void* args)
    {
        CB_ARGS* ca = (CB_ARGS*)args;

        if (NULL == ca)
            return -1L;
        while (true)
        {
            ca->lock();
            if (!ca->cb_.pop(&ca->fb_))
                ca->fb_.freeBlock();
            ca->unlock();
                //__cb_show_data((short*)fb.get(), fb.size());

            Sleep(10);
        }
        return 0L;
    }

    void 
    sl_network_circularbuffer(void)
    {
        int   i = 0;
        bool   mode = false;
        short dat[100] = {0};
        short spdat[640] = {0};
        struct CB_ARGS ca;


        ca.cb_.create(4096);

        CloseHandle(CreateThread(NULL, 0, __cbReadThread, &ca, 0L, NULL));
        CloseHandle(CreateThread(NULL, 0, __cbShowThread, &ca, 0L, NULL));
        while (true) 
        {
            ca.lock();
            if (!mode) 
            {
                srand((uint)time(0));
                for (i = 0; i < 100; ++i)
                    dat[i] = rand() % 100;
                ca.cb_.push((byte*)dat, sizeof(dat));
            } 
            else 
            {
                srand((uint)time(0));
                for (i = 0; i < 640; ++i)
                    spdat[i] = rand() % 640;
                ca.cb_.push((byte*)spdat, sizeof(spdat));
            }
            mode = !mode;

            srand((uint)GetTickCount());
            if (5 == rand() % 100)
            {
                fprintf(stdout, "\n\n ***************** clear circular buffer ******************** \n\n\n");
                ca.cb_.clear();
                Sleep(1000);
            }
            ca.unlock();

            Sleep(10);
        }

        ca.cb_.release();
    }

}
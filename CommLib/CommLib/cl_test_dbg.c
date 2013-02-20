/*
 * Copyright (c) 2010, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 *
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#include "cl_test.h"
#include "cl_dbg.h"
#include <windows.h>


static int g_isQuit = 0;

static unsigned long __stdcall _thread(void* _args) {
    SYSTEMTIME st = {0};

    while (1) {
        if (g_isQuit)
            break;
        GetLocalTime(&st);
        dbgPrint("Now time is : %.2d:%.2d:%.2d:%.3d", 
            st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

        Sleep(100);
    }
    g_isQuit = 0;
    dbgPrint("the thread quit in safe way ...");
    return 0L;
}



static void _cl_test_dbg_single(void) {
    int a = 23, b = 56;
    char* str = "This is the string test...";
    size_t len = strlen(str);

    dbgPrint("[%d] ADD [%d] = [%d]", a, b, a + b);
    dbgPrint("[%d] SUB [%d] = [%d]", a, b, a - b);
    dbgPrint("[%d] MUL [%d] = [%d]", a, b, a * b);
    dbgPrint("[%d] DIV [%d] = [%d]", a, b, a / b);
    dbgPrint("[%d] MOD [%d] = [%d]", a, b, a % b);

    dbgPrint("strig is : %s, length is : %d", str, len);
}

static void _cl_test_dbg_multi(void) {
    HANDLE h = CreateThread(NULL, 0, _thread, NULL, 0, NULL);
    if (NULL != h) {
        dbgPrint("Create the _thread success ...");
    } else {
        dbgPrint("Create the _thread failed ...");
    }

    if (NULL != h) {
        CloseHandle(h);
        h = NULL;
    }

    Sleep(2000);
    g_isQuit = 1;
    while (g_isQuit)
        Sleep(10);
}


void cl_test_dbg(void) {
    int sel = 0;

    while (1) {
        printf("*******************************************\n");
        printf("    0 - quit ...\n");
        printf("    1 - dbg functions in single thread ...\n");
        printf("    2 - dbg functions in multi-threads ...\n");
        printf("*******************************************\n");

        scanf("%d", &sel);
        if (0 == sel)
            break;
        switch (sel) {
        case 1: 
            _cl_test_dbg_single();
            break;
        case 2: 
            _cl_test_dbg_multi();
            break;
        default:
            printf("please enter another right selections ...\n\n\n");
        }
        printf("\n\n");
    }

}
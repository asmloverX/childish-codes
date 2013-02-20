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
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/timeb.h>
#include "cl_dbg.h"

#if _WIN32 || _WIN64
#define inline  __inline
#pragma warning(disable:4996)
#define __sleep(_millitm)   Sleep(_millitm)
#else
#define __sleep(_millitm)   usleep(_millitm * 1E3)
#endif


struct _DebugTime {
    unsigned char  _hour;
    unsigned char  _min;
    unsigned char  _sec;
    unsigned short _usec;
};

static int g_isTake = 0;


static inline void _getDebugTime(struct _DebugTime* _dtp) {
    struct timeb tb    = {0};
    struct tm*   local = 0;

    if (0 != _dtp) {
        ftime(&tb);
        local = localtime(&tb.time);
        _dtp->_hour = local->tm_hour;
        _dtp->_min  = local->tm_min;
        _dtp->_sec  = local->tm_sec;
        _dtp->_usec = tb.millitm;
    }
}

static inline int _debugPrintA(const char* _fname, 
    const char* _func, int _line, const char* _string) 
{
    FILE* _File = fopen("debug.log", "a+");
    struct _DebugTime dt = {0};
    char   string[1024]  = {0};

    if (NULL == _File || NULL == _fname || 
        NULL == _func || NULL == _string)
        return -1;
    _getDebugTime(&dt);
    sprintf(string, "[%.2d:%.2d:%.2d:%.3d]%s, %s[%d]: %s\n", 
        dt._hour, dt._min, dt._sec, dt._usec, 
        strrchr(_fname, '\\') + 1, _func, _line, _string);
    fprintf(_File, "%s", string);
    fflush(_File);
    printf("%s", string);
#if _WIN32 || _WIN64
    OutputDebugStringA(string);
#endif

    if (NULL != _File) {
        fclose(_File);
        _File = NULL;
    }
    return 0;
}

static inline int _debugInternalA(const char* _fname, 
    const char* _func, int _line, const char* _format, va_list _argList) 
{
    char buffer[1024] = {0};

    if (NULL == _fname || NULL == _func || 
        NULL == _format || NULL == _argList)
        return -1;
    vsprintf(buffer, _format, _argList);
    return _debugPrintA(_fname, _func, _line, buffer);
}


void 
dbgPrintA(const char* fname, const char* func, int line, const char* format, ...) {
    va_list argList = NULL;

    while (g_isTake)
        __sleep(100);
    g_isTake = 1;
    va_start(argList, format);
    _debugInternalA(fname, func, line, format, argList);
    va_end(argList);
    g_isTake = 0;
}
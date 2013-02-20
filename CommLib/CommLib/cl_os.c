/*
 * Copyright (c) 2010, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 *
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if _WIN32 || _WIN64
#define inline  __inline
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include "cl_os.h"


#if _WIN32 || _WIN64
#define __sleep(_millitm)   Sleep(_millitm)
#else 
#define __sleep(_millitm)   usleep(_millitm * 1E3)
#endif




void cl_sleep(unsigned long millitm) {
    __sleep(millitm);
}
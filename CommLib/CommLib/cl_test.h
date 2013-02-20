/*
 * Copyright (c) 2010, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 *
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#ifndef __CL_TEST_HEADER_H___
#define __CL_TEST_HEADER_H___

#if _WIN32 || _WIN64
#define inline __inline
#pragma warning(disable:4996)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern void cl_test_dbg(void);
extern void cl_test_thread(void);

#endif  /* __CL_TEST_HEADER_H___ */
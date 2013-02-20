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
#include "cl_config.h"
#include "cl_alloc.h"


void* __cl_object_new(void* __x, size_t __s) {
    void* __p = cl_malloc(__s);

    if ((void*)0 != __p && (void*)0 != __x) 
        memcpy(__p, __x, __s);
    return __p;
}

void __cl_object_free(void* __x, size_t __s) {
    cl_free(__x, __s);
}
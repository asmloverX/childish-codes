/*
 * Copyright (c) 2010, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 *
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#ifndef __CL_CONFIG_HEADER_H__
#define __CL_CONFIG_HEADER_H__

#if _WIN32 || _WIN64
#define inline __inline
#endif

typedef void* (*_cl_objectalloc_func)(void*);
typedef void (*_cl_objectfree_func)(void*);

typedef void (*_cl_objectoper_func)(void*);

struct cl_object_func {
    _cl_objectalloc_func _alloc_func;
    _cl_objectfree_func  _free_func;
};

static inline void* __cl_alloc_func(void* __x) {
    return __x;
}

static inline void __cl_free_func(void* __x) { }

extern void* __cl_object_new(void* __x, size_t __s);
extern void __cl_object_free(void* __x, size_t __s);

#endif  /* __CL_CONFIG_HEADER_H__ */
/*
 * Copyright (c) 2010, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 *
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#ifndef __CL_ALLOCATOR_HEADER_H__
#define __CL_ALLOCATOR_HEADER_H__

#define cl_memory_init   cl_alloc_init
#define cl_memory_uninit cl_alloc_uninit
#define cl_malloc(_n)    __cl_malloc(_n)
#define cl_free(_p, _s)  { if (0 != _p) { cl_dealloc(_p, _s); _p = 0; } }

extern int cl_alloc_init(void);
extern void cl_alloc_uninit(void);

extern void* cl_alloc(size_t size);
extern void cl_dealloc(void* p, size_t size);

extern void* __cl_malloc(size_t __s);

#endif  /* __CL_ALLOCATOR_HEADER_H__ */
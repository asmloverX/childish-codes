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
#if _WIN32 || _WIN64
#include <windows.h>
#endif
#include "cl_alloc.h"


#if _WIN32 || _WIN64
#define inline  __inline
#define __sleep(_millitm)   Sleep(_millitm)
#else
#define __sleep(_millitm)   usleep(_millitm * 1E3)
#endif


#define _ALIGN          (8)
#define _MAX_NUMBER     (64)
#define _CHUNK_LIMITS   (16385)
#define _CHUNK_NUMBER   (_CHUNK_LIMITS / _ALIGN)


struct __memory_list {
    struct __memory_list* _next;
};

struct __chunk_list {
    struct __chunk_list*  _next;
    struct __memory_list* _data;
};

struct __cl_alloc {
    struct __memory_list* _free_list[_CHUNK_NUMBER];
    long                  _guard[_CHUNK_NUMBER];
    struct __chunk_list*  _chunk_list;
    long                  _chunk_guard;
};


static struct __cl_alloc* g_instance            = 0;
static long               g_singleton_guard     = 0L;
static int                g_singleton_destroyed = 0;



static inline size_t _min(size_t _a, size_t _b) {
    return (_a < _b ? _a : _b);
}

static inline void _lock_on(long* _mutex) {
    while (InterlockedExchange(_mutex, 1L))
        __sleep(0);
}

static inline void _lock_off(long* _mutex) {
    InterlockedExchange(_mutex, 0L);
}

static inline size_t _chunk_index(size_t _bytes) {
    return ((_bytes - 1) / _ALIGN);
}

static inline struct __memory_list* 
_alloc_chunk(struct __cl_alloc* _p, size_t _index) {
    size_t node_sz  = (_index + 1) * _ALIGN;
    size_t chunk_sz = _min(_CHUNK_LIMITS / node_sz * node_sz, node_sz * _MAX_NUMBER);
    int    i        = 0;
    struct __memory_list*  ret = 0;
    struct __memory_list*  it  = 0;
    struct __memory_list** current_list = 0;

    if ((void*)0 == _p) 
        return (void*)0;
    _lock_on(&_p->_chunk_guard);
    current_list = &_p->_free_list[_index];
    if ((void*)0 != *current_list) {
        ret = *current_list;
        goto __quit;
    }
    ret = *current_list = (struct __memory_list*)malloc(chunk_sz);
    if ((void*)0 == ret) 
        goto __quit;
    it  = ret;
    for (i = 0; i < (int)(chunk_sz - node_sz * 2); i += node_sz)
        it = it->_next = it + (_index + 1) * _ALIGN / sizeof(*it);
    it->_next = (void*)0;

__quit:
    _lock_off(&_p->_chunk_guard);
    return ret;
}




int cl_alloc_init(void) {
    static struct __cl_alloc ca = {0};

    _lock_on(&g_singleton_guard);
    if ((void*)0 != g_instance)
        goto __quit;
    g_instance = &ca;
__quit:
    _lock_off(&g_singleton_guard);
    return 0;
}

void cl_alloc_uninit(void) {
    int i = 0, s = 0;
    void** chunk = 0;
    struct __chunk_list* temp = 0;

    if ((void*)0 == g_instance)
        return;
    temp = g_instance->_chunk_list;
    while ((void*)0 != temp) {
        s++;
        temp = temp->_next;
    }
    chunk = (void**)malloc(s * sizeof(void*));
    temp  = g_instance->_chunk_list;
    while ((void*)0 != temp) {
        chunk[i++] = temp->_data;
        temp = temp->_next;
    }
    for (i = 0; i < s; i++) 
        free(chunk[i]);
    free(chunk);

    g_singleton_destroyed = 1;
    g_instance = 0;
}

void* cl_alloc(size_t size) {
    void*  ret   = 0;
    size_t index = _chunk_index(size);
    struct __memory_list** temp = 0;

    if ((void*)0 == g_instance)
        return ret;
    _lock_on(&g_instance->_guard[index]);

    temp = &g_instance->_free_list[index];
    if ((void*)0 == *temp) {
        struct __memory_list* new_chunk  = _alloc_chunk(g_instance, index);
        struct __chunk_list*  chunk_node = 0;
        if (index == _chunk_index(sizeof(struct __chunk_list))) {
            chunk_node = (struct __chunk_list*)*temp;
            *temp = (*temp)->_next;
        } else {
            chunk_node = (struct __chunk_list*)cl_alloc(sizeof(struct __chunk_list));
        }
        _lock_on(&g_instance->_chunk_guard);
        chunk_node->_next = g_instance->_chunk_list;
        chunk_node->_data = new_chunk;
        g_instance->_chunk_list = chunk_node;
        _lock_off(&g_instance->_chunk_guard);
    }
    ret = *temp;
    *temp = (*temp)->_next;

    _lock_off(&g_instance->_guard[index]);
    return ret;
}

void cl_dealloc(void* p, size_t size) {
    size_t index = _chunk_index(size);
    struct __memory_list** temp       = 0;
    struct __memory_list*  free_block = (struct __memory_list*)p;
    
    if ((void*)0 == g_instance)
        return;
    _lock_on(&g_instance->_guard[index]);
    temp = &g_instance->_free_list[index];
    free_block->_next = *temp;
    *temp = free_block;
    _lock_off(&g_instance->_guard[index]);
}

void* __cl_malloc(size_t __s) {
    void* __p = cl_alloc(__s);
    
    if ((void*)0 != __p)
        memset(__p, 0, __s);
    return __p;
}
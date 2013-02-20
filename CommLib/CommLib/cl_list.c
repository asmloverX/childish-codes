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
#include "cl_alloc.h"
#include "cl_list.h"


struct __list_node {
    void* _data;
    struct __list_node* _prev;
    struct __list_node* _next;
};

struct cl_list {
    size_t               _size;
    struct __list_node   _node;
    _cl_objectalloc_func _alloc;
    _cl_objectfree_func  _free;
};


static inline struct __list_node* 
_list_new_node(struct cl_list* _p, void* _x) {
    struct __list_node* node = NULL;
    size_t size = sizeof(struct __list_node);

    if (NULL != _p) {
        node = (struct __list_node*)cl_malloc(size);
        if (NULL != node)
            node->_data = _p->_alloc(_x);
    }
    return node;
}

static inline int 
_list_insert(struct cl_list* _p, struct __list_node* _pos, void* _x) {
    struct __list_node* node = _list_new_node(_p, _x);

    if (NULL == _p || NULL == _pos || NULL == node)
        return -1;
    node->_prev = _pos->_prev;
    node->_next = _pos;
    _pos->_prev->_next = node;
    _pos->_prev = node;
    _p->_size++;
    return 0;
}

static inline int 
_list_erase(struct cl_list* _p, struct __list_node* _pos) {
    struct __list_node* prev = NULL;
    struct __list_node* next = NULL;

    if (NULL == _p || NULL == _pos)
        return -1;
    prev = _pos->_prev;
    next = _pos->_next;
    prev->_next = next;
    next->_prev = prev;
    if (NULL != _pos->_data)
        _p->_free(_pos->_data);
    cl_free(_pos, sizeof(struct __list_node));
    _p->_size--;
    return 0;
}



struct cl_list* cl_list_new(struct cl_object_func cof) {
    struct cl_list* self = NULL;
    size_t size = sizeof(struct cl_list);

    cl_memory_init();
    self = (struct cl_list*)cl_malloc(size);
    if (NULL != self) {
        self->_alloc = (NULL != cof._alloc_func ? cof._alloc_func : __cl_alloc_func);
        self->_free  = (NULL != cof._free_func ? cof._free_func : __cl_free_func);
        self->_node._prev = &self->_node;
        self->_node._next = &self->_node;
    } else {
        cl_memory_uninit();
    }
    return self;
}

void cl_list_free(struct cl_list** cllp) {
    cl_list_clear(*cllp);
    cl_free(*cllp, sizeof(struct cl_list));
    cl_memory_uninit();
}

size_t cl_list_size(struct cl_list* self) {
    return (NULL != self ? self->_size : 0);
}

int cl_list_empty(struct cl_list* self) {
    return (cl_list_begin(self) == cl_list_end(self));
}

void cl_list_clear(struct cl_list* self) {
    cl_list_iter i = cl_list_begin(self);
    struct __list_node* n = NULL;

    if (NULL == self || NULL == i) 
        return;
    while (i != cl_list_end(self)) {
        n = (struct __list_node*)i;
        i = cl_list_next(i);
        _list_erase(self, n);
    }
    self->_size = 0;
}

int cl_list_push_back(struct cl_list* self, void* data) {
    return _list_insert(self, (struct __list_node*)cl_list_end(self), data);
}

int cl_list_push_front(struct cl_list* self, void* data) {
    return _list_insert(self, (struct __list_node*)cl_list_begin(self), data);
}

int cl_list_insert(struct cl_list* self, cl_list_iter pos, void* data) {
    return _list_insert(self, (struct __list_node*)pos, data);
}

int cl_list_pop_back(struct cl_list* self) {
    struct __list_node* node = (struct __list_node*)cl_list_end(self);
    if (NULL != node)
        node = node->_prev;
    return _list_erase(self, node);
}

int cl_list_pop_front(struct cl_list* self) {
    return _list_erase(self, (struct __list_node*)cl_list_begin(self));
}

int cl_list_erase(struct cl_list* self, cl_list_iter pos) {
    return _list_erase(self, (struct __list_node*)pos);
}

void* cl_list_front(struct cl_list* self) {
    struct __list_node* node = (struct __list_node*)cl_list_begin(self);
    return (NULL != node ? node->_data : NULL);
}

void* cl_list_back(struct cl_list* self) {
    struct __list_node* node = (struct __list_node*)cl_list_end(self);
    if (NULL != node)
        node = node->_prev;
    return (NULL != node ? node->_data : NULL);
}

cl_list_iter cl_list_begin(struct cl_list* self) {
    return (NULL != self ? (cl_list_iter)self->_node._next : NULL);
}

cl_list_iter cl_list_end(struct cl_list* self) {
    return (NULL != self ? (cl_list_iter)&self->_node : NULL);
}

cl_list_iter cl_list_next(cl_list_iter current) {
    struct __list_node* it = (struct __list_node*)current;
    return (NULL != it ? (cl_list_iter)it->_next : NULL);
}

int cl_list_for_each(struct cl_list* self, _cl_objectoper_func func) {
    cl_list_iter i = cl_list_begin(self);
    struct __list_node* n = NULL;

    if (NULL == self || NULL == i)
        return -1;
    while (i != cl_list_end(self)) {
        n = (struct __list_node*)i;
        i = cl_list_next(i);
        if (NULL != func)
            func(n->_data);
    }
    return 0;
}
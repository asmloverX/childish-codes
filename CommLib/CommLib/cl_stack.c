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
#include "cl_stack.h"


struct __stack_node {
    void* _data;
    struct __stack_node* _next;
};

struct cl_stack {
    size_t               _top;
    struct __stack_node* _head;
    _cl_objectalloc_func _alloc;
    _cl_objectfree_func  _free;
};


static inline struct __stack_node* 
_stack_new_node(struct cl_stack* _p, void* _x) {
    struct __stack_node* node = NULL;
    size_t size = sizeof(struct __stack_node);

    if (NULL != _p) {
        node = (struct __stack_node*)cl_malloc(size);
        if (NULL != node)
            node->_data = _p->_alloc(_x);
    }
    return node;
}

static inline void _stack_clear(struct cl_stack* _p) {
    struct __stack_node* node = NULL;

    if (NULL == _p) 
        return;
    while (NULL != _p->_head) {
        node = _p->_head;
        _p->_head = _p->_head->_next;
        if (NULL != node->_data)
            _p->_free(node->_data);
        cl_free(node, sizeof(struct __stack_node));
    }
    _p->_top = 0;
}



struct cl_stack* cl_stack_new(struct cl_object_func cof) {
    struct cl_stack* self = NULL;
    size_t size = sizeof(struct cl_stack);

    cl_memory_init();
    self = (struct cl_stack*)cl_malloc(size);
    if (NULL != self) {
        self->_alloc = (NULL != cof._alloc_func ? cof._alloc_func : __cl_alloc_func);
        self->_free  = (NULL != cof._free_func ? cof._free_func : __cl_free_func);
    } else {
        cl_memory_uninit();
    }
    return self;
}

void cl_stack_free(struct cl_stack** clsp) {
    _stack_clear(*clsp);
    cl_free(*clsp, sizeof(struct cl_stack));
    cl_memory_uninit();
}

size_t cl_stack_size(struct cl_stack* self) {
    return (NULL != self ? self->_top : 0);
}

int cl_stack_empty(struct cl_stack* self) {
    return (NULL != self ? NULL == self->_head : 0);
}

int cl_stack_push(struct cl_stack* self, void* data) {
    struct __stack_node* node = _stack_new_node(self, data);

    if (NULL == self || NULL == node)
        return -1;
    node->_next = self->_head;
    self->_head = node;
    self->_top++;
    return 0;
}

int cl_stack_pop(struct cl_stack* self) {
    struct __stack_node* node = NULL;

    if (NULL == self)
        return -1;
    if (NULL != self->_head) {
        node = self->_head;
        self->_head = self->_head->_next;
        if (NULL != node->_data) 
            self->_free(node->_data);
        cl_free(node, sizeof(struct __stack_node));
        self->_top--;
    }
    return 0;
}

void* cl_stack_top(struct cl_stack* self) {
    struct __stack_node* node = NULL;

    if (NULL == self)
        return NULL;
    node = self->_head;
    return (NULL != node ? node->_data : NULL);
}
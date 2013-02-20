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
#include "cl_queue.h"


struct __queue_node {
    void* _data;
    struct __queue_node* _next;
};

struct cl_queue {
    size_t               _size;
    struct __queue_node* _head;
    struct __queue_node* _tail;
    _cl_objectalloc_func _alloc;
    _cl_objectfree_func  _free;
};


static inline struct __queue_node* 
_queue_new_node(struct cl_queue* _p, void* _x) {
    struct __queue_node* node = NULL;
    size_t size = sizeof(struct __queue_node);

    if (NULL != _p) {
        node = (struct __queue_node*)cl_malloc(size);
        if (NULL != node) 
            node->_data = _p->_alloc(_x);
    }
    return node;
}

static inline void _queue_clear(struct cl_queue* _p) {
    struct __queue_node* node = NULL;

    if (NULL == _p)
        return;
    while (NULL != _p->_head) {
        node = _p->_head;
        _p->_head = _p->_head->_next;
        if (NULL != node->_data)
            _p->_free(node->_data);
        cl_free(node, sizeof(struct __queue_node));
    }
    _p->_size = 0;
}



struct cl_queue* cl_queue_new(struct cl_object_func cof) {
    struct cl_queue* self = NULL;
    size_t size = sizeof(struct cl_queue);

    cl_memory_init();
    self = (struct cl_queue*)cl_malloc(size);
    if (NULL != self) {
        self->_alloc = (NULL != cof._alloc_func ? cof._alloc_func : __cl_alloc_func);
        self->_free  = (NULL != cof._free_func ? cof._free_func : __cl_free_func);
    } else {
        cl_memory_uninit();
    }
    return self;
}

void cl_queue_free(struct cl_queue** clqp) {
    _queue_clear(*clqp);
    cl_free(*clqp, sizeof(struct cl_queue));
    cl_memory_uninit();
}

size_t cl_queue_size(struct cl_queue* self) {
    return (NULL != self ? self->_size : 0);
}

int cl_queue_empty(struct cl_queue* self) {
    return (NULL != self ? NULL == self->_head : 0);
}

int cl_queue_push(struct cl_queue* self, void* data) {
    struct __queue_node* node = _queue_new_node(self, data);

    if (NULL == self || NULL == node)
        return -1;
    if (NULL == self->_head && NULL == self->_tail)
        self->_head = self->_tail = node;
    else {
        self->_tail->_next = node;
        self->_tail = node;
    }
    self->_size++;
    return 0;
}

int cl_queue_pop(struct cl_queue* self) {
    struct __queue_node* node = NULL;

    if (NULL == self)
        return -1;
    if (NULL != self->_head) {
        node = self->_head;
        self->_head = self->_head->_next;
        if (NULL != node->_data)
            self->_free(node->_data);
        cl_free(node, sizeof(struct __queue_node));
        self->_size--;
    }
    return 0;
}

void* cl_queue_top(struct cl_queue* self) {
    struct __queue_node* node = NULL;

    if (NULL == self)
        return NULL;
    node = self->_head;
    return (NULL != node ? node->_data : NULL);
}
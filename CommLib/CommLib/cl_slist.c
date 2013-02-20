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
#include "cl_slist.h"


struct __slist_node {
    void* _data;
    struct __slist_node* _next;
};

struct cl_slist {
    size_t               _size;
    struct __slist_node* _head;
    struct __slist_node* _tail;
    _cl_objectalloc_func _alloc;
    _cl_objectfree_func  _free;
};


static inline struct __slist_node* 
_slist_new_node(struct cl_slist* _p, void* _x) {
    struct __slist_node* node = NULL;
    size_t size = sizeof(struct __slist_node);

    if (NULL != _p) {
        node = (struct __slist_node*)cl_malloc(size);
        if (NULL != node) 
            node->_data = _p->_alloc(_x);
    }
    return node;
}



struct cl_slist* cl_slist_new(struct cl_object_func cof) {
    struct cl_slist* self = NULL;
    size_t size = sizeof(struct cl_slist);

    cl_memory_init();
    self = (struct cl_slist*)cl_malloc(size);
    if (NULL != self) {
        self->_alloc = (NULL != cof._alloc_func ? cof._alloc_func : __cl_alloc_func);
        self->_free  = (NULL != cof._free_func ? cof._free_func : __cl_free_func);
    } else {
        cl_memory_uninit();
    }
    return self;
}

void cl_slist_free(struct cl_slist** clsp) {
    cl_slist_clear(*clsp);
    cl_free(*clsp, sizeof(struct cl_slist));
    cl_memory_uninit();
}

size_t cl_slist_size(struct cl_slist* self) {
    return (NULL != self ? self->_size : 0);
}

int cl_slist_empty(struct cl_slist* self) {
    return (NULL != self ? NULL == self->_head : 0);
}

void cl_slist_clear(struct cl_slist* self) {
    struct __slist_node* n = NULL;

    if (NULL == self) 
        return;
    while (NULL != self->_head) {
        n = self->_head;
        self->_head = self->_head->_next;
        if (NULL != n->_data)
            self->_free(n->_data);
        cl_free(n, sizeof(struct __slist_node));
    }
    self->_size = 0;
}

int cl_slist_push_back(struct cl_slist* self, void* data) {
    struct __slist_node* node = _slist_new_node(self, data);

    if (NULL == self || NULL == node)
        return -1;
    if (NULL == self->_head || NULL == self->_tail)
        self->_head = self->_tail = node;
    else {
        self->_tail->_next = node;
        self->_tail = node;
    }
    self->_size++;
    return 0;
}

int cl_slist_push_front(struct cl_slist* self, void* data) {
    struct __slist_node* node = _slist_new_node(self, data);

    if (NULL == self || NULL == node)
        return -1;
    node->_next = self->_head;
    self->_head = node;
    self->_size++;
    return 0;
}

int cl_slist_pop_front(struct cl_slist* self) {
    struct __slist_node* node = NULL;

    if (NULL == self)
        return -1;
    if (NULL != self->_head) {
        node = self->_head;
        self->_head = self->_head->_next;
        if (NULL != node->_data)
            self->_free(node->_data);
        cl_free(node, sizeof(struct __slist_node));
        self->_size--;
    }
    return 0;
}

void* cl_slist_front(struct cl_slist* self) {
    struct __slist_node* node = NULL;

    if (NULL == self)
        return NULL;
    node = self->_head;
    return (NULL != node ? node->_data : NULL);
}

void* cl_slist_back(struct cl_slist* self) {
    struct __slist_node* node = NULL;

    if (NULL == self)
        return NULL;
    node = self->_tail;
    return (NULL != node ? node->_data : NULL);
}

cl_slist_iter cl_slist_begin(struct cl_slist* self) {
    return (NULL != self ? (cl_slist_iter)self->_head : NULL);
}

cl_slist_iter cl_slist_end(struct cl_slist* self) {
    return NULL;
}

cl_slist_iter cl_slist_next(cl_slist_iter current) {
    struct __slist_node* it = (struct __slist_node*)current;

    return (NULL != it ? (cl_slist_iter)it->_next : NULL);
}

int cl_clist_for_each(struct cl_slist* self, _cl_objectoper_func func) {
    struct __slist_node* node = NULL;

    if (NULL == self)
        return -1;
    node = self->_head;
    while (NULL != node) {
        if (NULL != func)
            func(node->_data);
        node = node->_next;
    }
    return 0;
}
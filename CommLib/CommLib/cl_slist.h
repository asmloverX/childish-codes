/*
 * Copyright (c) 2010, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 *
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#ifndef __CL_SLIST_HEADER_H__
#define __CL_SLIST_HEADER_H__

#include "cl_config.h"

typedef void** cl_slist_iter;
struct cl_slist;

extern struct cl_slist* cl_slist_new(struct cl_object_func cof);
extern void cl_slist_free(struct cl_slist** clsp);

extern size_t cl_slist_size(struct cl_slist* self);
extern int cl_slist_empty(struct cl_slist* self);

extern void cl_slist_clear(struct cl_slist* self);
extern int cl_slist_push_back(struct cl_slist* self, void* data);
extern int cl_slist_push_front(struct cl_slist* self, void* data);
extern int cl_slist_pop_front(struct cl_slist* self);
extern void* cl_slist_front(struct cl_slist* self);
extern void* cl_slist_back(struct cl_slist* self);

extern cl_slist_iter cl_slist_begin(struct cl_slist* self);
extern cl_slist_iter cl_slist_end(struct cl_slist* self);
extern cl_slist_iter cl_slist_next(cl_slist_iter current);

extern int cl_clist_for_each(struct cl_slist* self, _cl_objectoper_func func);

#endif  /* __CL_SLIST_HEADER_H__ */
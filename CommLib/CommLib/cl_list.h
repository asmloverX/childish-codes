/*
 * Copyright (c) 2010, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 *
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#ifndef __CL_LIST_HEADER_H__
#define __CL_LIST_HEADER_H__

#include "cl_config.h"

typedef void** cl_list_iter;
struct cl_list;

extern struct cl_list* cl_list_new(struct cl_object_func cof);
extern void cl_list_free(struct cl_list** cllp);

extern size_t cl_list_size(struct cl_list* self);
extern int cl_list_empty(struct cl_list* self);

extern void cl_list_clear(struct cl_list* self);
extern int cl_list_push_back(struct cl_list* self, void* data);
extern int cl_list_push_front(struct cl_list* self, void* data);
extern int cl_list_insert(struct cl_list* self, cl_list_iter pos, void* data);
extern int cl_list_pop_back(struct cl_list* self);
extern int cl_list_pop_front(struct cl_list* self);
extern int cl_list_erase(struct cl_list* self, cl_list_iter pos);

extern cl_list_iter cl_list_begin(struct cl_list* self);
extern cl_list_iter cl_list_end(struct cl_list* self);
extern cl_list_iter cl_list_next(cl_list_iter current);

extern void* cl_list_front(struct cl_list* self);
extern void* cl_list_back(struct cl_list* self);

extern int cl_list_for_each(struct cl_list* self, _cl_objectoper_func func);

#endif  /* __CL_LIST_HEADER_H__ */
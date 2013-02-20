/*
 * Copyright (c) 2010, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 *
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#ifndef __CL_STACK_HEADER_H__
#define __CL_STACK_HEADER_H__

#include "cl_config.h"

struct cl_stack;

extern struct cl_stack* cl_stack_new(struct cl_object_func cof);
extern void cl_stack_free(struct cl_stack** clsp);

extern size_t cl_stack_size(struct cl_stack* self);
extern int cl_stack_empty(struct cl_stack* self);

extern int cl_stack_push(struct cl_stack* self, void* data);
extern int cl_stack_pop(struct cl_stack* self);
extern void* cl_stack_top(struct cl_stack* self);

#endif  /* __CL_STACK_HEADER_H__ */
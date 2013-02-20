/*
 * Copyright (c) 2010, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 *
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#ifndef __CL_QUEUE_HEADER_H__
#define __CL_QUEUE_HEADER_H__

#include "cl_config.h"

struct cl_queue;

extern struct cl_queue* cl_queue_new(struct cl_object_func cof);
extern void cl_queue_free(struct cl_queue** clqp);

extern size_t cl_queue_size(struct cl_queue* self);
extern int cl_queue_empty(struct cl_queue* self);

extern int cl_queue_push(struct cl_queue* self, void* data);
extern int cl_queue_pop(struct cl_queue* self);
extern void* cl_queue_top(struct cl_queue* self);

#endif  /* __CL_QUEUE_HEADER_H__ */
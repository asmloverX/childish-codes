/*
 * sl_lock_doc.h - document for class sl::Lock
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved.
 * 
 * created at 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * sl::Lock - 锁的操作类, 以便进行对各种原子操作的加锁/解锁
 *      在Windows下使用的是CRITICAL_SECTION, 在Linux下使用的是
 *      pthread_mutex_t
 * 
 *
 * 例子: 参见(../test/sl_network_test/sl_network_test_lock.cc)
 */
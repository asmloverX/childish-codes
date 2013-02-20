/*
 * sl_circularbuffer_doc.h - document for class sl::CircularBUffer
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved.
 * 
 * created at 2011/01/14 17:15:09 by ASMlover.
 */
/*
 * sl::CircularBuffer - 内存循环缓冲区操作类, 该类是对sl::Buffer类
 *      的简单封装操作, 该类保证每一次读取得到的都是一个完整的
 *      数据包, 同时对原子操作push和pop加上了锁操作, 以供多线程
 *      的操作环境。
 * 
 *      该内存循环缓冲区的内部存储空间操作原理是: 在push新的数据
 *      到循环缓冲区的时候, 先检查剩余的空间是否可以保存该完整的
 *      数据包, 如果不能的话, 就先读取完整的数据直到剩余空间可以
 *      存放该新数据的时候再将新的数据push到循环内存缓冲区中。
 *      在pop数据到外部的buffer的时候, 先读取sizeof(size_t)长度的数
 *      据来得知该完整数据包的长度信息, 再根据得知的数据长度来读
 *      取该完整的数据包数据。
 * 
 *      
 *      class CircularBuffer {
 *          sl::Lock*   l_; // 多线程操作锁
 *          sl::Buffer* b_; // 内存缓冲区操作类
 *              ...
 *      };
 * 
 *      @在CircularBuffer对象构造成功之后创建该循环内存空间
 *          成功返回true, 否则返回false
 *      bool create(sl::size_t bufLen = 1024);
 * 
 *      @在CircularBuffer析构之前调用release成员函数来释放对应
 *          的循环缓冲区内存空间
 *      void release(void);
 *
 *      @调用clear成员函数清空CircularBuffer对象的内存数据, 将
 *          缓冲区的数据信息重新设置为0, 同时重置读写指针的位置
 *      void clear(void);
 * 
 *      @调用push成员函数来将一个完整的inLen自己数据信息从
 *          inBuf中push到循环缓冲区当中, 在循环缓冲区当中是先
 *          将inLen的值, 然后再是存放的inBuf中的数据信息. 如果
 *          没有错误发生就返回true, 否则返回false
 *      bool push(const sl::byte* inBuf, sl::size_t inLen);
 *
 *      @该函数于其重载的函数功能相似
 *      bool push(const sl::FrameBlock* inBlock);
 * 
 *      @调用pop成员函数来读取一个完整的帧数据包. 在循环缓冲
 *          区中先读取sizeof(size_t)长度的变量来得知具体的完整
 *          数据帧的长度, 再根据该长度来读取完整的数据帧放在
 *          FrameBlock对象当中.
 *      @return<bool>: 从循环缓冲区pop数据成功返回true, 否则返回false
 *      bool pop(sl::FrameBlock* outBlock);
 */
/*
 * sl_buffer_doc.h - document for class sl::Buffer
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved.
 * 
 * created by 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * sl::Buffer - 内存缓冲区操作类, 主要用来将存放网络层接收到的
 *      数据信息, 以便减少内存空间的频繁申请和释放, 减少内存
 *      操作带来的性能消耗和内存碎片
 *      
 *      sl::Buffer的内存模型如下: ===标识写入的数据
 *                                  ---标识空闲的空间
 *
 *      (read-pos/write-pos)
 *      |-------------------------------------------------------|
 *
 *                          (read-pos)              (write-pos)
 *      |-------------------========================------------|
 *
 *               (write-pos)                           (read-pos)
 *      |========--------------------------------------=========|
 *
 *       (read-pos)                         (write-pos)
 *      |===================================--------------------|
 *
 *       (read-pos)                                             (write-pos)
 *      |=======================================================|
 *
 *                                  (write-pos/read-pos)
 *      |===========================|===========================|
 *
 * 
 *      class Buffer {
 *          byte* buf_;         // Buffer类中具体存储数据的内存空间
 *                              // 该内存空间应该在对象创建的时候一并申请
 *          size_t readPos_;    // 当前读取到的数据位置
 *          size_t writePos_;   // 当前写的数据位置
 *          size_t bufLen_;     // 对象创建的时候所申请的内存空间的长度
 *              ...
 *      };
 * 
 *      @调用create成员函数来创建Buffer循环内存存储空间, 默认创建
 *          的内存存储空间的长度为1024字节
 *      @param [in] bufLen<const sl::size_t&>: 创建的内存空间的长度
 *          如果用户传入的空间长度小于1024便默认使用1024字节
 *      @return<bool>: 创建内存空间成功返回true, 否则返回false
 *      bool create(const sl::size_t& bufLen = 1024);
 *
 *      @在使用完后Buffer对象进行操作后, 需要调用release成员函数来
 *          释放所申请的循环存储内存空间
 *      void release(void);
 * 
 *      @调用clear成员函数来清空buffer的存储缓冲区, 重新设置读写
 *          的指针位置为0
 *      void clear(void);
 * 
 *      @将inLen字节的inBuf中的数据写入到Buffer内存空间当中
 *      @param [in] inBuf<const sl::byte*>: 写入到Buffer内存空间中的数据
 *      @param [in] inLen<sl::size_t>: 写入到Buffer内存空间中的数据的长度
 *      @return<sl::size_t>: 写入数据成功返回所写入的数据信息的字节数, 
 *          返回值为0标识写入数据失败或者Buffer没有空间可供写入
 *      sl::size_t write(const sl::byte* inBuf, sl::size_t inLen);
 * 
 *      @从Buffer的内存空间中读取outLen字节的数据到outBuf中
 *      @param [in] outLen<sl::size_t>: 读取的数据的字节数
 *      @param [out] outBuf<sl::byte*>: 从Buffer内存空间中所读取到的数据
 *      @return<sl::size_t>: 返回从Buffer的循环内存空间中所读取到的数据长度, 
 *          返回值为0时标识读取数据信息失败或者Buffer内存空间中没有数据可读
 *      sl::size_t read(sl::size_t outLen, sl::byte* outBuf);
 * 
 *      @从Buffer的内存空间中删除len字节的数据信息, 也就是将读取数据的
 *          位置指针在Buffer的内存空间中向后移动len字节
 *      @param [in] len<sl::size_t>: 移除的字节数
 *      @return<sl::size_t>: 返回从Buffer的循环内存空间中移除的数据长度, 
 *          返回值为0的时候表示Buffer的内存空间中没有数据可读或remove失败
 *      sl::size_t remove(sl::size_t len);
 *
 *      
 *  例子: 参见(../test/sl_network_test/sl_net_work_buffer.cc)
 */
/*
 * sl_frame_doc.h - document for class sl::FrameBlock
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved.
 * 
 * created at 2011/01/14 16:54:46 by ASMlover.
 */
/*
 * sl::FrameBlock - 完整的整帧数据包操作类, 主要用来从
 *      循环缓冲区中获取完整的一包数据, 减少了客户自己
 *      判断、解析一个完整的数据包的大小的工作。同时将
 *      出错的机会隐藏在FrameBlock的内部
 * 
 *      一个完整的数据包的内存模型如下所示:
 *            SL_Msghead             full frame block data
 *      |---------------------|===============================|
 *      那么FrameBlock中所保存的数据长度为SL_MsgHead的长度加上
 *      后面所跟的数据信息的长度的总和, 保存的数据则是该完整
 *      的数据包
 * 
 * 
 *      class FrameBlock {
 *          sl::size_t size_t_; // 完整的数据包的长度信息
 *          sl::byte*  data_;   // 完整的数据包数据信息
 *              ...
 *      };
 * 
 *      @在FrameBlock对象构造之后可以调用该allocBlock函数来
 *          为即将设置的帧数据申请内存空间
 *      @param [in] blockLen<sl::size_t>: 申请的内存空间长度
 *      @return<bool>: 成功返回true, 否则返回false
 *      bool allocBlock(sl::size_t blockLen);
 * 
 *      @该函数于其重载函数功能类似, 其还要做的操作是将inLen
 *          字节的数据信息从inBuf中拷贝到FrameBlock对象的内存空间
 *      @param [in] inBuf<const sl::byte*>:  需要拷贝进FrameBlock
 *          对象的内存空间的完整的数据包信息
 *      @param [in] inLen<sl::size_t>: 拷贝的数据信息长度
 *      @return<bool>: 成功返回true, 否则返回false
 *      bool allocBlock(const sl::byte* inBuf, sl::size_t inLen);
 * 
 *      @在不使用该FrameBlock对象的时候调用freeBlock成员函数
 *          来释放其内存存储空间
 *      void freeBlock(void);
 * 
 *      @调用isValid成员函数来得知该对象的数据信息是否有效
 *      bool isValid(void) const;
 * 
 *      @调用get成员函数从FrameBlock对象的内存空间中拷贝
 *          outLen字节的数据到outBuf中, 如果成功返回true, 否则返回false
 *      bool get(sl::size_t outLen, sl::byte* outBuf);
 * 
 *      @调用该重载的get成员函数来获取FrameBlock内部的数据信息
 *      const sl::byte* get(void) const;
 *
 * 
 * 例子: 参见(../test/sl_network_test/sl_network_test_frame.cc)
 */
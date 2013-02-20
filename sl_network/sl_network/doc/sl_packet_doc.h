/*
 * sl_packet_doc.h - document for class sl::SubPacket/Packet
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved.
 * 
 * created at 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * sl::SubPacket - 网络传送的每一个子数据包的处理类, 
 *      便于统一对整个完整数据包的管理, 同时也可以将各个
 *      模块的职能更为清晰的分开, 起到解耦的作用
 * 
 * 
 *      class SubPacket {
 *          sl::ushort sessionId_;  // 该子数据包所标识的会话ID
 *          sl::ushort count_;      // 该子数据包标识的完整数据包所分为的子数据包数量
 *          sl::ushort index_;      // 该会话ID所指定的子数据包的序号(从1开始)
 *          sl::ushort size_;       // 该会话ID所指定的子数据包的字节数
 *          sl::byte*  data_;       // 该会话ID所指定的子数据包的具体数据信息
 *              ... 
 *      };
 * 
 *      @调用setValue成员函数来设置该子数据包的数据信息, 以及该数据包
 *          对应的相关数据包信息标识-会话ID等
 *      @param [in] np<const struct sl::NetProtocol*>: 网络数据信息的协议头信息
 *      @param [in] data<const sl::byte*>: 该子数据包具体的数据信息
 *      @return<bool>: 设置该子数据包的数据信息成功返回true, 出错的话会返回false
 *      bool setValue(const struct sl::NetProtocol* np, const sl::byte* data);
 * 
 *      @该函数与上一个函数的作用类似, 只是设置子数据包的值的参数不同而已
 *      bool setValue(
 *          const sl::ushort& sessionId, 
 *          const sl::ushort& count, 
 *          const sl::ushort& index, 
 *          const sl::ushort& size, 
 *          const sl::byte* data
 *          );
 *
 * 
 * sl::Packet - 主要是对子数据包的管理, 针对从网络另一端接收到的数据信息
 *      组合成一个完整的数据包信息, 以便业务处理的模块进行对应的处理, 同
 *      时将各个模块之间的耦合性进一步降低
 * 
 * 
 *      class Packet {
 *          sl::ushort sessionId_;  // 该完整的数据信息包的会话ID
 *          sl::ulong  size_;       // 该完整数据信息包完整数据信息的字节数
 *          sl::byte*  data_;       // 该完整数据包的数据信息
 *              ... 
 *      };
 *
 *      @在构建一个Packet对象之后调用create成员函数来创建该
 *          完整数据包的存储空间, 使用该函数之后需要在不使用该Packet对象
 *          之后调用release来释放掉对应申请的内存存储空间
 *      @param [in] subPackCount<sl::ushort>: 该完整数据包所包含的
 *          子数据包数, 默认每个完整的数据包必须包含一个子数据包
 *      @return<bool>: 如果没有错误或异常发生, 该函数返回true, 否则返回false
 *      bool create(sl::ushort subPackCount = 1);
 * 
 *      @在析构Packet对象之前需要调用release成员函数来释放内存空间
 *      void release(void);
 * 
 *      @调用addSubPacket成员函数来向Packet对象中添加新的子数据包, 完整的
 *          数据包由Packet内部自己来重组
 *      @return<bool>: 如果没有错误发生, 该函数返回truem 否则返回false
 *      bool addSubPacket(const sl::SubPacket& sp);
 * 
 *      @从Packet的内存空间中读取size字节的数据信息到data中
 *      @return<bool>: 成功返回true, 否则返回false
 *      bool getData(sl::ulong size, sl::byte* data);
 *
 *      @该函数与上一个的作用相似, 是直接返回的该完整数据信息的首地址
 *          需要用户自己使用类似memcpy的函数拷贝到相应的buffer当中
 *      const sl::byte* getData(void) const;
 * 
 * 
 * 例子: 参见(../test/sl_network_test/sl_network_test_packet.cc)
 */
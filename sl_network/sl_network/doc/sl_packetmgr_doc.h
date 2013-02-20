/*
 * sl_packetmgr_doc.h - document for class sl::PacketMgr
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved. 
 * 
 * created at 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * sl::PacketMgr - 该类主要是对网络两端的数据信息进行分包/组包
 *      的管理, 对需要分包的数据进行分包(后续相应的处理交给客户)
 *      同时对接收到的网络另一端的数据信息组合成一个完整的数据
 *      包(完整包的对应处理也是交由给客户)
 * 
 *      该类内部对所接收到的Packet是使用hash_map来进行管理的, 以便
 *      快速的将新接收到的子数据包插入到的相同会话ID的Packet中
 * 
 *
 *      class PacketMgr {
 *          void* packMap_;         // 接收到的完整数据包的hash_map队列
 *          sl::ushort sessionId_;  // 主要用于生成分包的包会话ID
 *              ...
 *      };
 * 
 *      @在构造一个PacketMgr对象之后调用init成员函数来进行初始化操作
 *      bool init(void);
 *
 *      @在析构一个PacketMgr对象之前调用uninit成员函数来进行反初始化操作
 *      void uninit(void);
 * 
 *      @调用subpackage成员函数来对需要分包处理的数据信息进行分包
 *          具体的后续处理交由回调函数spFN来处理, 同时该回调函数
 *          spFN主要是由客户来实现的, 客户可以将分包后的数据信息
 *          发送到网络另一端等 ... ...
 *      bool subpackage(
 *          const sl::byte* data, 
 *          const sl::ulong& size, 
 *          bool (*spFN)(const sl::byte* outBuf, sl::ushort size, void* args), 
 *          void* args
 *          );
 * 
 *      @调用combination成员函数来对接收到的网络数据包进行重新组合
 *          数据包的处理, 当组合成一个完整的数据包之后的后续操作主要
 *          是交由回调函数fpFN来处理的, 同时该回调函数fpFN主要是由客户
 *          来实现的, 客户将重组后的完整数据信息交由业务处理模块来
 *          进行业务处理等 ... ...
 *      bool combination(
 *          const sl::NetProtocol* np, 
 *          const sl::byte* data, 
 *          bool (*fpFN)(const sl::byte* outBuf, sl::ulong size, void* args), 
 *          void* args
 *          );
 * 
 *
 * 例子: 参看(../test/sl_network_test/sl_network_test_packetmgr.cc)
 */
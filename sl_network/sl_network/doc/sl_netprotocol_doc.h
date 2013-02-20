/*
 * sl_netprotocol_doc.h - document for network transport
 *      protocol(struct sl::NetProtocol).
 * n
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved. 
 * 
 * created at 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * sl::NetProtocol - 数据信息在各个网络对象之间相互传送的
 *      数据协议头标识, 该协议便于网络的一端接收相应长度
 *      的数据信息, 以保证每一包数据的完整性, 从而提高网
 *      络之间的通信传输性能
 * 
 *      网络一端发送/接收的一个完整的网络包的内存模型如下:
 *          NetProtocol           packet data
 *      |------------------|========================|
 * 
 *      如果网络之间发送的一个完整的数据包过长的话, 需要对该
 *      数据信息进行分包处理(按每包768字节处理), 每一个完整的
 *      数据包应该统一由一个会话ID来标识, 而对应的每一个子数
 *      据包都拥有该统一的会话ID。网络接收该数据包的另一端根据
 *      该会话ID来重组该完整的数据信息包。每一个子数据包的内存
 *      模型如下所示:
 *          NetProtocol      sub packet data
 *      |------------------|==========|
 *      
 *      需要注意的是该网络传输协议头也应该以1字节对齐, 以便网络
 *      另一端接收到数据的正确性。
 * 
 * 
 *      struct NetProtocol {
 *          sl::ushort signature_;  // 网络传输协议头的标识, 用0xA9EE来填写
 *          union {
 *              sl::ushort ver_;
 *              struct { sl::byte low_, high_; } sVer_;
 *          } version_;             // 网络传输协议头版本, 用0x0100来填写
 *          sl::ushort packSessionId_;  // 当前数据信息的会话ID
 *          sl::ushort subPackCount_;   // 该数据信息总共分为的子数据包数
 *          sl::ushort subPackIndex_;   // 当前该子数据包的序号(从1开始)
 *          sl::ushort subPacketsz_;    // 当前该子数据包的字节长度
 *      };
 * 
 * 
 * 例子: 参见(
 *             ../inc/sl_packet.h 
 *             ../src/sl_packet.cc
 *             ../inc/sl_packetmgr.h
 *             ../inc/sl_packetmgr.cc
 *             )
 */
/*
 * sl_msgprotocol_doc.h - document for Protocol of 
 *      Message (struct SL_MsgHead)
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved. 
 * 
 * created at 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * SL_MsgHead - 网络一端发送或是接收到的需要处理的数据信息的
 *      一个信息头标识, 主要用于标识该信息主要是用于干什么用
 *      抑或是需要做什么动作等, 需要注意的是在网络中传送该
 *      结构的时候该结构应该以1字节对齐的方式传送
 *
 *      网络一端发送或接收的一个完整的数据信息的内存模型如下:
 *          SL_MsgHead                   data
 *      |---------------|=======================================|
 * 
 *      
 *      struct SL_MsgHead {
 *          ushort cbsize_;     // 该SL_MsgHead结构的字节长度信息
 *          union {
 *              dword_t dw_;
 *              struct { word_t main_, sub_; } w_;
 *          } cmd_;             // 发送/接收的数据信息的信息业务标识
 *                              //      main_标识了主命令类型, sub_为子命令类型
 *                              //      在发送信息的时候需要填写main_/sub_的信息
 *          ushort channelId_;  // 该数据信息需要操作的信道标识, 没有操作
 *                              //      信道的时候需要用0xFFFF来填写
 *          
 *          // 其他的为保留参数, 具体使用需要看具体的业务中的描述
 *              ...
 *      };
 * 
 * 
 * 例子: 参见(
 *             ../test/sl_network_test/sl_network_test_capp.cc
 *             ../test/sl_network_test/sl_network_test_sapp.cc
 *             )
 */
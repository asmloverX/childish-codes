/*
 * sl_socket_doc.h - document for class sl::SocketHandler
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved. 
 * 
 * created at 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * sl::SocketHandler - 该类主要是封装了对SOCKET进行调用的API, 
 *      封装一些SOCKET的基本操作, 以便于加速对网络模块的开发。
 *      同时该类是一个基类, 需要客户继承来处理具体关心的网
 *      络事件
 * 
 *      class SocketHandler {
 *          sl::socket_type s_; // SOCKET句柄信息
 *          void*           h_; // SocketHandler对应的内部窗口句柄
 *              ... 
 *      };
 *
 *      @在构造一个SocketHandler对象之后调用create成员函数来创建一个
 *          SOCKET句柄信息, 如果没有错误出现返回true, 否则返回false
 *          需要注意的是在不需要使用该SocketHandler对象之后调用
 *          close来关掉该SOCKET句柄信息
 *      bool create(void);
 *
 *      @调用connectTo成员函数来对指定IP地址和端口号的SocketHandler
 *          建立一个连接
 *          注意该函数会立即返回, 并不会等到连接成功之后才返回, 
 *          是否连接成功我们需要在FD_CONNECT事件处理函数eventConnect
 *          中来得知和处理
 *      bool connectTo(const char* address, sl::ushort port);
 * 
 *      @调用acceptFrom成员函数来接受网络另一端新连接, 连接成功
 *          会返回所接受的连接的地址信息以及SocketHandler
 *      @return<bool>: 如果没有错误发生返回true, 否则返回false
 *      bool acceptFrom(struct sockaddr* from, sl::SocketHandler* sh);
 * 
 *      @调用read成员函数从该SocketHandler上接收len字节的数据信息
 *          当网络某一端发生异常或断开的时候返回false, 接收数据
 *          成功返回true
 *      bool read(int len, char* outBuf);
 * 
 *      @调用write成员函数从该SocketHandler上发送len字节长度的数据
 *          信息inBuf到网络另一端,  该函数在网络另一端没有断开, 连接
 *          良好的情况下会一直等到网络另一端完全接收完数据后返回true, 
 *          当网络状况出现异常或错误的时候返回false
 *      bool write(const char* inBuf, int len);
 * 
 *      @调用selEvent来申请该SocketHandler需要通知/关心的网络事件
 *      bool selEvent(long sEvent, sl::uint msg = sl::UM_DISPATCH);
 * 
 *
 *      @该函数由框架在通知socket handler连接成功的时候调用, 同理该
 *          函数需要由SocketHandler的继承类来具体实现
 *      virtual void eventConnect(sl::SocketHandler* sh);
 * 
 *      @该函数由框架在通知该SocketHandler网络另一端发出连接请求
 *          的时候调用, 然后可以调用acceptFrom来接受客户端的连接请求, 
 *          同时该函数也需要由SocketHandler的继承类来具体实现
 *      virtual void eventAccept(sl::SocketHandler* sh);
 * 
 *      @该函数由框架在通知该SocketHandler可以接收网络另一端数据
 *          的时候调用, 我们使用read函数来接收网络另一端的数据信息
 *          该函数由继承类来具体实现
 *      virtual void eventRead(sl::SocketHandler* sh);
 * 
 *      @该函数由框架在通知该SocketHandler现在可以调用write来向网络
 *          另一端发送数据信息的时候调用, 同时该函数由继承类来实现
 *      virtual void eventWrite(sl::SocketHandler* sh);
 * 
 *      @该函数在框架通知该SocketHandler的连接断开的时候调用
 *      virtual void eventClose(sl::SocketHandler* sh);
 * 
 * 
 * 例子: 参见(
 *             ../test/sl_network_test/sl_network_test_capp.cc
 *             ../test/sl_network_test/sl_network_test_sapp.cc
 *             ../test/sl_network_test/sl_network_test_client.cc
 *             ../test/sl_network_test/sl_network_test_server.cc
 *             )
 */
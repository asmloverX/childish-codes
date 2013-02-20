/*
 * sl_dispatch_doc.h - document for class sl::Dispatch
 *      sl::Dispatch_C/sl::Dispatch_S
 *
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved. 
 * 
 * created at 2011/01/12 12:06:21 by ASMlover.
 */
/*
 * sl::Dispatch - SocketHandler的调度操作类, 主要进行SocketHandler
 *      的相关调度, 启动或停止SocketHandler以及调度各种网络事件
 *      如: 接收客户连接事件/连接服务成功事件/读写事件/关闭事件
 *
 *      class sl::Dispatch类之间调度关系如下:
 *          Dispatch ---------------------->SocketHandler
 *             △                            △
 *            /  \                               |
 *   Dispatch_C  Dispatch_S              ExtendSocketHandler
 * 
 * 
 *      class Dispatch {
 *          SocketHandler*  sh_;        // 设置到Dispatch中的SocketHandler对象
 *                                      // 主要是调用该SocketHandler对象进行相应的操作
 *          sl::hwnd_type   h_;         // 内部窗口的句柄信息
 *          sl::handle_type hThread_;   // 内部窗口的线程句柄信息
 *          sl::handle_type hEvent_;    // 内部窗口线程相关的事件
 *          sl::sl_char     sName_[50]; // 内部窗口的窗口类型名称
 *              ...
 *      };
 * 
 *      @调用setHandler成员函数设置SocketHandler到Dispatch中以便进行对应的调度
 *      @param [in] sh<sl::SocketHandler>: 设置到Dispatch中的SocketHandler对象
 *      @return<bool>: 设置SocketHandler对象成功返回true, 否则返回false
 *      bool setSHandler(sl::SocketHandler* sh);
 * 
 *      @获取当前Dispatch中的SocketHandler对象
 *      sl::SocketHandler* getSHandler(void) const;
 * 
 *      @调用begin成员函数来启动一个SocketHandler的调度, 同时该接口
 *          由其继承类来具体实现, 处理客户SocketHandler/服务端SocketHandler
 *          的具体调度操作
 *      @param [in] address<const char*>: 具体的网络地址信息-IP地址
 *      @param [in] port<sl::ushort>: 连接的网络另一端的端口号
 *      @return<bool>: 开始调度成功返回true, 否则返回false
 *      virtual bool begin(const char* address, sl::ushort port);
 * 
 *      @调用end成员函数来结束一个SocketHandler的调度, 该接口由其继承类
 *          来继承实现, 停止服务端SocketHandler/客户端SocketHandler的调度
 *      virtual void end(void);
 * 
 *      @sThread是实现对SocketHandler进行各种网络事件的调度的窗口线程函数
 *          在sThread内部实现内部隐藏窗口的创建以及消息循环等
 *      static sl::return_type sl::CALLBACK sThread(void* args);
 * 
 *      @createMsgLoop函数主要实现内部隐藏窗口的消息循环的调度线程, 创建
 *          成功返回true, 否则返回false
 *      bool createMsgLoop(void);
 * 
 *      @waitForMsgLoopRun函数主要等待内部隐藏的窗口线程的运行, 等待其
 *          开始进行消息循环的时候, 其他线程方可进行相应的操作
 *      @param [in] millitm<sl::ulong>: 等待的时间(单位: ms)
 *      @return<bool>: 等待成功返回true, 出错的情况下返回false
 *      bool waitForMsgLoopRun(sl::ulong millitm);
 * 
 *      @windowNew主要用来创建一个窗口对象, 同时返回该窗口的句柄标识信息
 *          出错或者失败的情况下返回NULL
 *      sl::hwnd_type windowNew(void);
 *
 *      @调用getUuidStr成员函数来获取一个UUID的字符串信息, 该信息用作
 *          窗口的类型名称, 如果成功返回true, 否则返回false
 *      bool getUuidStr(sl::size_t srtLen, sl::sl_char* outStr);
 * 
 *      @dispEventProc函数主要实现对所绑定的SocketHandler的各种网络操作事件
 *          的调度, 例如调度FD_ACCEPT/FD_CONNECT/FD_READ/FD_WRITE/FD_CLOSE等
 *      long dispEventProc(sl::wparam_type wp, sl::lparam_type);
 *
 * 
 * 例子: 参见(
 *              ../test/sl_network_test/sl_network_test_capp.cc
 *              ../test/sl_network_test/sl_network_test_sapp.cc
 *              ../test/sl_network_test/sl_network_client.cc
 *              ../test/sl_network_test/sl_network_server.cc
 *             )
 */
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
 * sl::Dispatch - SocketHandler�ĵ��Ȳ�����, ��Ҫ����SocketHandler
 *      ����ص���, ������ֹͣSocketHandler�Լ����ȸ��������¼�
 *      ��: ���տͻ������¼�/���ӷ���ɹ��¼�/��д�¼�/�ر��¼�
 *
 *      class sl::Dispatch��֮����ȹ�ϵ����:
 *          Dispatch ---------------------->SocketHandler
 *             ��                            ��
 *            /  \                               |
 *   Dispatch_C  Dispatch_S              ExtendSocketHandler
 * 
 * 
 *      class Dispatch {
 *          SocketHandler*  sh_;        // ���õ�Dispatch�е�SocketHandler����
 *                                      // ��Ҫ�ǵ��ø�SocketHandler���������Ӧ�Ĳ���
 *          sl::hwnd_type   h_;         // �ڲ����ڵľ����Ϣ
 *          sl::handle_type hThread_;   // �ڲ����ڵ��߳̾����Ϣ
 *          sl::handle_type hEvent_;    // �ڲ������߳���ص��¼�
 *          sl::sl_char     sName_[50]; // �ڲ����ڵĴ�����������
 *              ...
 *      };
 * 
 *      @����setHandler��Ա��������SocketHandler��Dispatch���Ա���ж�Ӧ�ĵ���
 *      @param [in] sh<sl::SocketHandler>: ���õ�Dispatch�е�SocketHandler����
 *      @return<bool>: ����SocketHandler����ɹ�����true, ���򷵻�false
 *      bool setSHandler(sl::SocketHandler* sh);
 * 
 *      @��ȡ��ǰDispatch�е�SocketHandler����
 *      sl::SocketHandler* getSHandler(void) const;
 * 
 *      @����begin��Ա����������һ��SocketHandler�ĵ���, ͬʱ�ýӿ�
 *          ����̳���������ʵ��, ����ͻ�SocketHandler/�����SocketHandler
 *          �ľ�����Ȳ���
 *      @param [in] address<const char*>: ����������ַ��Ϣ-IP��ַ
 *      @param [in] port<sl::ushort>: ���ӵ�������һ�˵Ķ˿ں�
 *      @return<bool>: ��ʼ���ȳɹ�����true, ���򷵻�false
 *      virtual bool begin(const char* address, sl::ushort port);
 * 
 *      @����end��Ա����������һ��SocketHandler�ĵ���, �ýӿ�����̳���
 *          ���̳�ʵ��, ֹͣ�����SocketHandler/�ͻ���SocketHandler�ĵ���
 *      virtual void end(void);
 * 
 *      @sThread��ʵ�ֶ�SocketHandler���и��������¼��ĵ��ȵĴ����̺߳���
 *          ��sThread�ڲ�ʵ���ڲ����ش��ڵĴ����Լ���Ϣѭ����
 *      static sl::return_type sl::CALLBACK sThread(void* args);
 * 
 *      @createMsgLoop������Ҫʵ���ڲ����ش��ڵ���Ϣѭ���ĵ����߳�, ����
 *          �ɹ�����true, ���򷵻�false
 *      bool createMsgLoop(void);
 * 
 *      @waitForMsgLoopRun������Ҫ�ȴ��ڲ����صĴ����̵߳�����, �ȴ���
 *          ��ʼ������Ϣѭ����ʱ��, �����̷߳��ɽ�����Ӧ�Ĳ���
 *      @param [in] millitm<sl::ulong>: �ȴ���ʱ��(��λ: ms)
 *      @return<bool>: �ȴ��ɹ�����true, ���������·���false
 *      bool waitForMsgLoopRun(sl::ulong millitm);
 * 
 *      @windowNew��Ҫ��������һ�����ڶ���, ͬʱ���ظô��ڵľ����ʶ��Ϣ
 *          �������ʧ�ܵ�����·���NULL
 *      sl::hwnd_type windowNew(void);
 *
 *      @����getUuidStr��Ա��������ȡһ��UUID���ַ�����Ϣ, ����Ϣ����
 *          ���ڵ���������, ����ɹ�����true, ���򷵻�false
 *      bool getUuidStr(sl::size_t srtLen, sl::sl_char* outStr);
 * 
 *      @dispEventProc������Ҫʵ�ֶ����󶨵�SocketHandler�ĸ�����������¼�
 *          �ĵ���, �������FD_ACCEPT/FD_CONNECT/FD_READ/FD_WRITE/FD_CLOSE��
 *      long dispEventProc(sl::wparam_type wp, sl::lparam_type);
 *
 * 
 * ����: �μ�(
 *              ../test/sl_network_test/sl_network_test_capp.cc
 *              ../test/sl_network_test/sl_network_test_sapp.cc
 *              ../test/sl_network_test/sl_network_client.cc
 *              ../test/sl_network_test/sl_network_server.cc
 *             )
 */
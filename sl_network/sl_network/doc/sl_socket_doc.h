/*
 * sl_socket_doc.h - document for class sl::SocketHandler
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved. 
 * 
 * created at 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * sl::SocketHandler - ������Ҫ�Ƿ�װ�˶�SOCKET���е��õ�API, 
 *      ��װһЩSOCKET�Ļ�������, �Ա��ڼ��ٶ�����ģ��Ŀ�����
 *      ͬʱ������һ������, ��Ҫ�ͻ��̳������������ĵ���
 *      ���¼�
 * 
 *      class SocketHandler {
 *          sl::socket_type s_; // SOCKET�����Ϣ
 *          void*           h_; // SocketHandler��Ӧ���ڲ����ھ��
 *              ... 
 *      };
 *
 *      @�ڹ���һ��SocketHandler����֮�����create��Ա����������һ��
 *          SOCKET�����Ϣ, ���û�д�����ַ���true, ���򷵻�false
 *          ��Ҫע������ڲ���Ҫʹ�ø�SocketHandler����֮�����
 *          close���ص���SOCKET�����Ϣ
 *      bool create(void);
 *
 *      @����connectTo��Ա��������ָ��IP��ַ�Ͷ˿ںŵ�SocketHandler
 *          ����һ������
 *          ע��ú�������������, ������ȵ����ӳɹ�֮��ŷ���, 
 *          �Ƿ����ӳɹ�������Ҫ��FD_CONNECT�¼�������eventConnect
 *          ������֪�ʹ���
 *      bool connectTo(const char* address, sl::ushort port);
 * 
 *      @����acceptFrom��Ա����������������һ��������, ���ӳɹ�
 *          �᷵�������ܵ����ӵĵ�ַ��Ϣ�Լ�SocketHandler
 *      @return<bool>: ���û�д���������true, ���򷵻�false
 *      bool acceptFrom(struct sockaddr* from, sl::SocketHandler* sh);
 * 
 *      @����read��Ա�����Ӹ�SocketHandler�Ͻ���len�ֽڵ�������Ϣ
 *          ������ĳһ�˷����쳣��Ͽ���ʱ�򷵻�false, ��������
 *          �ɹ�����true
 *      bool read(int len, char* outBuf);
 * 
 *      @����write��Ա�����Ӹ�SocketHandler�Ϸ���len�ֽڳ��ȵ�����
 *          ��ϢinBuf��������һ��,  �ú�����������һ��û�жϿ�, ����
 *          ���õ�����»�һֱ�ȵ�������һ����ȫ���������ݺ󷵻�true, 
 *          ������״�������쳣������ʱ�򷵻�false
 *      bool write(const char* inBuf, int len);
 * 
 *      @����selEvent�������SocketHandler��Ҫ֪ͨ/���ĵ������¼�
 *      bool selEvent(long sEvent, sl::uint msg = sl::UM_DISPATCH);
 * 
 *
 *      @�ú����ɿ����֪ͨsocket handler���ӳɹ���ʱ�����, ͬ���
 *          ������Ҫ��SocketHandler�ļ̳���������ʵ��
 *      virtual void eventConnect(sl::SocketHandler* sh);
 * 
 *      @�ú����ɿ����֪ͨ��SocketHandler������һ�˷�����������
 *          ��ʱ�����, Ȼ����Ե���acceptFrom�����ܿͻ��˵���������, 
 *          ͬʱ�ú���Ҳ��Ҫ��SocketHandler�ļ̳���������ʵ��
 *      virtual void eventAccept(sl::SocketHandler* sh);
 * 
 *      @�ú����ɿ����֪ͨ��SocketHandler���Խ���������һ������
 *          ��ʱ�����, ����ʹ��read����������������һ�˵�������Ϣ
 *          �ú����ɼ̳���������ʵ��
 *      virtual void eventRead(sl::SocketHandler* sh);
 * 
 *      @�ú����ɿ����֪ͨ��SocketHandler���ڿ��Ե���write��������
 *          ��һ�˷���������Ϣ��ʱ�����, ͬʱ�ú����ɼ̳�����ʵ��
 *      virtual void eventWrite(sl::SocketHandler* sh);
 * 
 *      @�ú����ڿ��֪ͨ��SocketHandler�����ӶϿ���ʱ�����
 *      virtual void eventClose(sl::SocketHandler* sh);
 * 
 * 
 * ����: �μ�(
 *             ../test/sl_network_test/sl_network_test_capp.cc
 *             ../test/sl_network_test/sl_network_test_sapp.cc
 *             ../test/sl_network_test/sl_network_test_client.cc
 *             ../test/sl_network_test/sl_network_test_server.cc
 *             )
 */
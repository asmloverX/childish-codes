/*
 * sl_packetmgr_doc.h - document for class sl::PacketMgr
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved. 
 * 
 * created at 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * sl::PacketMgr - ������Ҫ�Ƕ��������˵�������Ϣ���зְ�/���
 *      �Ĺ���, ����Ҫ�ְ������ݽ��зְ�(������Ӧ�Ĵ������ͻ�)
 *      ͬʱ�Խ��յ���������һ�˵�������Ϣ��ϳ�һ������������
 *      ��(�������Ķ�Ӧ����Ҳ�ǽ��ɸ��ͻ�)
 * 
 *      �����ڲ��������յ���Packet��ʹ��hash_map�����й����, �Ա�
 *      ���ٵĽ��½��յ��������ݰ����뵽����ͬ�ỰID��Packet��
 * 
 *
 *      class PacketMgr {
 *          void* packMap_;         // ���յ����������ݰ���hash_map����
 *          sl::ushort sessionId_;  // ��Ҫ�������ɷְ��İ��ỰID
 *              ...
 *      };
 * 
 *      @�ڹ���һ��PacketMgr����֮�����init��Ա���������г�ʼ������
 *      bool init(void);
 *
 *      @������һ��PacketMgr����֮ǰ����uninit��Ա���������з���ʼ������
 *      void uninit(void);
 * 
 *      @����subpackage��Ա����������Ҫ�ְ������������Ϣ���зְ�
 *          ����ĺ��������ɻص�����spFN������, ͬʱ�ûص�����
 *          spFN��Ҫ���ɿͻ���ʵ�ֵ�, �ͻ����Խ��ְ����������Ϣ
 *          ���͵�������һ�˵� ... ...
 *      bool subpackage(
 *          const sl::byte* data, 
 *          const sl::ulong& size, 
 *          bool (*spFN)(const sl::byte* outBuf, sl::ushort size, void* args), 
 *          void* args
 *          );
 * 
 *      @����combination��Ա�������Խ��յ����������ݰ������������
 *          ���ݰ��Ĵ���, ����ϳ�һ�����������ݰ�֮��ĺ���������Ҫ
 *          �ǽ��ɻص�����fpFN�������, ͬʱ�ûص�����fpFN��Ҫ���ɿͻ�
 *          ��ʵ�ֵ�, �ͻ�������������������Ϣ����ҵ����ģ����
 *          ����ҵ����� ... ...
 *      bool combination(
 *          const sl::NetProtocol* np, 
 *          const sl::byte* data, 
 *          bool (*fpFN)(const sl::byte* outBuf, sl::ulong size, void* args), 
 *          void* args
 *          );
 * 
 *
 * ����: �ο�(../test/sl_network_test/sl_network_test_packetmgr.cc)
 */
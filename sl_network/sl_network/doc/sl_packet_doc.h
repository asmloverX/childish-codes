/*
 * sl_packet_doc.h - document for class sl::SubPacket/Packet
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved.
 * 
 * created at 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * sl::SubPacket - ���紫�͵�ÿһ�������ݰ��Ĵ�����, 
 *      ����ͳһ�������������ݰ��Ĺ���, ͬʱҲ���Խ�����
 *      ģ���ְ�ܸ�Ϊ�����ķֿ�, �𵽽��������
 * 
 * 
 *      class SubPacket {
 *          sl::ushort sessionId_;  // �������ݰ�����ʶ�ĻỰID
 *          sl::ushort count_;      // �������ݰ���ʶ���������ݰ�����Ϊ�������ݰ�����
 *          sl::ushort index_;      // �ûỰID��ָ���������ݰ������(��1��ʼ)
 *          sl::ushort size_;       // �ûỰID��ָ���������ݰ����ֽ���
 *          sl::byte*  data_;       // �ûỰID��ָ���������ݰ��ľ���������Ϣ
 *              ... 
 *      };
 * 
 *      @����setValue��Ա���������ø������ݰ���������Ϣ, �Լ������ݰ�
 *          ��Ӧ��������ݰ���Ϣ��ʶ-�ỰID��
 *      @param [in] np<const struct sl::NetProtocol*>: ����������Ϣ��Э��ͷ��Ϣ
 *      @param [in] data<const sl::byte*>: �������ݰ������������Ϣ
 *      @return<bool>: ���ø������ݰ���������Ϣ�ɹ�����true, ����Ļ��᷵��false
 *      bool setValue(const struct sl::NetProtocol* np, const sl::byte* data);
 * 
 *      @�ú�������һ����������������, ֻ�����������ݰ���ֵ�Ĳ�����ͬ����
 *      bool setValue(
 *          const sl::ushort& sessionId, 
 *          const sl::ushort& count, 
 *          const sl::ushort& index, 
 *          const sl::ushort& size, 
 *          const sl::byte* data
 *          );
 *
 * 
 * sl::Packet - ��Ҫ�Ƕ������ݰ��Ĺ���, ��Դ�������һ�˽��յ���������Ϣ
 *      ��ϳ�һ�����������ݰ���Ϣ, �Ա�ҵ�����ģ����ж�Ӧ�Ĵ���, ͬ
 *      ʱ������ģ��֮�������Խ�һ������
 * 
 * 
 *      class Packet {
 *          sl::ushort sessionId_;  // ��������������Ϣ���ĻỰID
 *          sl::ulong  size_;       // ������������Ϣ������������Ϣ���ֽ���
 *          sl::byte*  data_;       // ���������ݰ���������Ϣ
 *              ... 
 *      };
 *
 *      @�ڹ���һ��Packet����֮�����create��Ա������������
 *          �������ݰ��Ĵ洢�ռ�, ʹ�øú���֮����Ҫ�ڲ�ʹ�ø�Packet����
 *          ֮�����release���ͷŵ���Ӧ������ڴ�洢�ռ�
 *      @param [in] subPackCount<sl::ushort>: ���������ݰ���������
 *          �����ݰ���, Ĭ��ÿ�����������ݰ��������һ�������ݰ�
 *      @return<bool>: ���û�д�����쳣����, �ú�������true, ���򷵻�false
 *      bool create(sl::ushort subPackCount = 1);
 * 
 *      @������Packet����֮ǰ��Ҫ����release��Ա�������ͷ��ڴ�ռ�
 *      void release(void);
 * 
 *      @����addSubPacket��Ա��������Packet����������µ������ݰ�, ������
 *          ���ݰ���Packet�ڲ��Լ�������
 *      @return<bool>: ���û�д�����, �ú�������truem ���򷵻�false
 *      bool addSubPacket(const sl::SubPacket& sp);
 * 
 *      @��Packet���ڴ�ռ��ж�ȡsize�ֽڵ�������Ϣ��data��
 *      @return<bool>: �ɹ�����true, ���򷵻�false
 *      bool getData(sl::ulong size, sl::byte* data);
 *
 *      @�ú�������һ������������, ��ֱ�ӷ��صĸ�����������Ϣ���׵�ַ
 *          ��Ҫ�û��Լ�ʹ������memcpy�ĺ�����������Ӧ��buffer����
 *      const sl::byte* getData(void) const;
 * 
 * 
 * ����: �μ�(../test/sl_network_test/sl_network_test_packet.cc)
 */
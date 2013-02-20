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
 * sl::NetProtocol - ������Ϣ�ڸ����������֮���໥���͵�
 *      ����Э��ͷ��ʶ, ��Э����������һ�˽�����Ӧ����
 *      ��������Ϣ, �Ա�֤ÿһ�����ݵ�������, �Ӷ������
 *      ��֮���ͨ�Ŵ�������
 * 
 *      ����һ�˷���/���յ�һ����������������ڴ�ģ������:
 *          NetProtocol           packet data
 *      |------------------|========================|
 * 
 *      �������֮�䷢�͵�һ�����������ݰ������Ļ�, ��Ҫ�Ը�
 *      ������Ϣ���зְ�����(��ÿ��768�ֽڴ���), ÿһ��������
 *      ���ݰ�Ӧ��ͳһ��һ���ỰID����ʶ, ����Ӧ��ÿһ������
 *      �ݰ���ӵ�и�ͳһ�ĻỰID��������ո����ݰ�����һ�˸���
 *      �ûỰID�������������������Ϣ����ÿһ�������ݰ����ڴ�
 *      ģ��������ʾ:
 *          NetProtocol      sub packet data
 *      |------------------|==========|
 *      
 *      ��Ҫע����Ǹ����紫��Э��ͷҲӦ����1�ֽڶ���, �Ա�����
 *      ��һ�˽��յ����ݵ���ȷ�ԡ�
 * 
 * 
 *      struct NetProtocol {
 *          sl::ushort signature_;  // ���紫��Э��ͷ�ı�ʶ, ��0xA9EE����д
 *          union {
 *              sl::ushort ver_;
 *              struct { sl::byte low_, high_; } sVer_;
 *          } version_;             // ���紫��Э��ͷ�汾, ��0x0100����д
 *          sl::ushort packSessionId_;  // ��ǰ������Ϣ�ĻỰID
 *          sl::ushort subPackCount_;   // ��������Ϣ�ܹ���Ϊ�������ݰ���
 *          sl::ushort subPackIndex_;   // ��ǰ�������ݰ������(��1��ʼ)
 *          sl::ushort subPacketsz_;    // ��ǰ�������ݰ����ֽڳ���
 *      };
 * 
 * 
 * ����: �μ�(
 *             ../inc/sl_packet.h 
 *             ../src/sl_packet.cc
 *             ../inc/sl_packetmgr.h
 *             ../inc/sl_packetmgr.cc
 *             )
 */
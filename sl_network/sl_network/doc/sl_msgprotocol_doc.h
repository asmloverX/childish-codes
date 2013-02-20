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
 * SL_MsgHead - ����һ�˷��ͻ��ǽ��յ�����Ҫ�����������Ϣ��
 *      һ����Ϣͷ��ʶ, ��Ҫ���ڱ�ʶ����Ϣ��Ҫ�����ڸ�ʲô��
 *      �ֻ�����Ҫ��ʲô������, ��Ҫע������������д��͸�
 *      �ṹ��ʱ��ýṹӦ����1�ֽڶ���ķ�ʽ����
 *
 *      ����һ�˷��ͻ���յ�һ��������������Ϣ���ڴ�ģ������:
 *          SL_MsgHead                   data
 *      |---------------|=======================================|
 * 
 *      
 *      struct SL_MsgHead {
 *          ushort cbsize_;     // ��SL_MsgHead�ṹ���ֽڳ�����Ϣ
 *          union {
 *              dword_t dw_;
 *              struct { word_t main_, sub_; } w_;
 *          } cmd_;             // ����/���յ�������Ϣ����Ϣҵ���ʶ
 *                              //      main_��ʶ������������, sub_Ϊ����������
 *                              //      �ڷ�����Ϣ��ʱ����Ҫ��дmain_/sub_����Ϣ
 *          ushort channelId_;  // ��������Ϣ��Ҫ�������ŵ���ʶ, û�в���
 *                              //      �ŵ���ʱ����Ҫ��0xFFFF����д
 *          
 *          // ������Ϊ��������, ����ʹ����Ҫ�������ҵ���е�����
 *              ...
 *      };
 * 
 * 
 * ����: �μ�(
 *             ../test/sl_network_test/sl_network_test_capp.cc
 *             ../test/sl_network_test/sl_network_test_sapp.cc
 *             )
 */
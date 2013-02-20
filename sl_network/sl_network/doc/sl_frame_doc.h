/*
 * sl_frame_doc.h - document for class sl::FrameBlock
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved.
 * 
 * created at 2011/01/14 16:54:46 by ASMlover.
 */
/*
 * sl::FrameBlock - ��������֡���ݰ�������, ��Ҫ������
 *      ѭ���������л�ȡ������һ������, �����˿ͻ��Լ�
 *      �жϡ�����һ�����������ݰ��Ĵ�С�Ĺ�����ͬʱ��
 *      ����Ļ���������FrameBlock���ڲ�
 * 
 *      һ�����������ݰ����ڴ�ģ��������ʾ:
 *            SL_Msghead             full frame block data
 *      |---------------------|===============================|
 *      ��ôFrameBlock������������ݳ���ΪSL_MsgHead�ĳ��ȼ���
 *      ����������������Ϣ�ĳ��ȵ��ܺ�, ������������Ǹ�����
 *      �����ݰ�
 * 
 * 
 *      class FrameBlock {
 *          sl::size_t size_t_; // ���������ݰ��ĳ�����Ϣ
 *          sl::byte*  data_;   // ���������ݰ�������Ϣ
 *              ...
 *      };
 * 
 *      @��FrameBlock������֮����Ե��ø�allocBlock������
 *          Ϊ�������õ�֡���������ڴ�ռ�
 *      @param [in] blockLen<sl::size_t>: ������ڴ�ռ䳤��
 *      @return<bool>: �ɹ�����true, ���򷵻�false
 *      bool allocBlock(sl::size_t blockLen);
 * 
 *      @�ú����������غ�����������, �仹Ҫ���Ĳ����ǽ�inLen
 *          �ֽڵ�������Ϣ��inBuf�п�����FrameBlock������ڴ�ռ�
 *      @param [in] inBuf<const sl::byte*>:  ��Ҫ������FrameBlock
 *          ������ڴ�ռ�����������ݰ���Ϣ
 *      @param [in] inLen<sl::size_t>: ������������Ϣ����
 *      @return<bool>: �ɹ�����true, ���򷵻�false
 *      bool allocBlock(const sl::byte* inBuf, sl::size_t inLen);
 * 
 *      @�ڲ�ʹ�ø�FrameBlock�����ʱ�����freeBlock��Ա����
 *          ���ͷ����ڴ�洢�ռ�
 *      void freeBlock(void);
 * 
 *      @����isValid��Ա��������֪�ö����������Ϣ�Ƿ���Ч
 *      bool isValid(void) const;
 * 
 *      @����get��Ա������FrameBlock������ڴ�ռ��п���
 *          outLen�ֽڵ����ݵ�outBuf��, ����ɹ�����true, ���򷵻�false
 *      bool get(sl::size_t outLen, sl::byte* outBuf);
 * 
 *      @���ø����ص�get��Ա��������ȡFrameBlock�ڲ���������Ϣ
 *      const sl::byte* get(void) const;
 *
 * 
 * ����: �μ�(../test/sl_network_test/sl_network_test_frame.cc)
 */
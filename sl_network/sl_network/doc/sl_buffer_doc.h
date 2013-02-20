/*
 * sl_buffer_doc.h - document for class sl::Buffer
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved.
 * 
 * created by 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * sl::Buffer - �ڴ滺����������, ��Ҫ����������������յ���
 *      ������Ϣ, �Ա�����ڴ�ռ��Ƶ��������ͷ�, �����ڴ�
 *      �����������������ĺ��ڴ���Ƭ
 *      
 *      sl::Buffer���ڴ�ģ������: ===��ʶд�������
 *                                  ---��ʶ���еĿռ�
 *
 *      (read-pos/write-pos)
 *      |-------------------------------------------------------|
 *
 *                          (read-pos)              (write-pos)
 *      |-------------------========================------------|
 *
 *               (write-pos)                           (read-pos)
 *      |========--------------------------------------=========|
 *
 *       (read-pos)                         (write-pos)
 *      |===================================--------------------|
 *
 *       (read-pos)                                             (write-pos)
 *      |=======================================================|
 *
 *                                  (write-pos/read-pos)
 *      |===========================|===========================|
 *
 * 
 *      class Buffer {
 *          byte* buf_;         // Buffer���о���洢���ݵ��ڴ�ռ�
 *                              // ���ڴ�ռ�Ӧ���ڶ��󴴽���ʱ��һ������
 *          size_t readPos_;    // ��ǰ��ȡ��������λ��
 *          size_t writePos_;   // ��ǰд������λ��
 *          size_t bufLen_;     // ���󴴽���ʱ����������ڴ�ռ�ĳ���
 *              ...
 *      };
 * 
 *      @����create��Ա����������Bufferѭ���ڴ�洢�ռ�, Ĭ�ϴ���
 *          ���ڴ�洢�ռ�ĳ���Ϊ1024�ֽ�
 *      @param [in] bufLen<const sl::size_t&>: �������ڴ�ռ�ĳ���
 *          ����û�����Ŀռ䳤��С��1024��Ĭ��ʹ��1024�ֽ�
 *      @return<bool>: �����ڴ�ռ�ɹ�����true, ���򷵻�false
 *      bool create(const sl::size_t& bufLen = 1024);
 *
 *      @��ʹ�����Buffer������в�����, ��Ҫ����release��Ա������
 *          �ͷ��������ѭ���洢�ڴ�ռ�
 *      void release(void);
 * 
 *      @����clear��Ա���������buffer�Ĵ洢������, �������ö�д
 *          ��ָ��λ��Ϊ0
 *      void clear(void);
 * 
 *      @��inLen�ֽڵ�inBuf�е�����д�뵽Buffer�ڴ�ռ䵱��
 *      @param [in] inBuf<const sl::byte*>: д�뵽Buffer�ڴ�ռ��е�����
 *      @param [in] inLen<sl::size_t>: д�뵽Buffer�ڴ�ռ��е����ݵĳ���
 *      @return<sl::size_t>: д�����ݳɹ�������д���������Ϣ���ֽ���, 
 *          ����ֵΪ0��ʶд������ʧ�ܻ���Bufferû�пռ�ɹ�д��
 *      sl::size_t write(const sl::byte* inBuf, sl::size_t inLen);
 * 
 *      @��Buffer���ڴ�ռ��ж�ȡoutLen�ֽڵ����ݵ�outBuf��
 *      @param [in] outLen<sl::size_t>: ��ȡ�����ݵ��ֽ���
 *      @param [out] outBuf<sl::byte*>: ��Buffer�ڴ�ռ�������ȡ��������
 *      @return<sl::size_t>: ���ش�Buffer��ѭ���ڴ�ռ�������ȡ�������ݳ���, 
 *          ����ֵΪ0ʱ��ʶ��ȡ������Ϣʧ�ܻ���Buffer�ڴ�ռ���û�����ݿɶ�
 *      sl::size_t read(sl::size_t outLen, sl::byte* outBuf);
 * 
 *      @��Buffer���ڴ�ռ���ɾ��len�ֽڵ�������Ϣ, Ҳ���ǽ���ȡ���ݵ�
 *          λ��ָ����Buffer���ڴ�ռ�������ƶ�len�ֽ�
 *      @param [in] len<sl::size_t>: �Ƴ����ֽ���
 *      @return<sl::size_t>: ���ش�Buffer��ѭ���ڴ�ռ����Ƴ������ݳ���, 
 *          ����ֵΪ0��ʱ���ʾBuffer���ڴ�ռ���û�����ݿɶ���removeʧ��
 *      sl::size_t remove(sl::size_t len);
 *
 *      
 *  ����: �μ�(../test/sl_network_test/sl_net_work_buffer.cc)
 */
/*
 * sl_circularbuffer_doc.h - document for class sl::CircularBUffer
 * 
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved.
 * 
 * created at 2011/01/14 17:15:09 by ASMlover.
 */
/*
 * sl::CircularBuffer - �ڴ�ѭ��������������, �����Ƕ�sl::Buffer��
 *      �ļ򵥷�װ����, ���ౣ֤ÿһ�ζ�ȡ�õ��Ķ���һ��������
 *      ���ݰ�, ͬʱ��ԭ�Ӳ���push��pop������������, �Թ����߳�
 *      �Ĳ���������
 * 
 *      ���ڴ�ѭ�����������ڲ��洢�ռ����ԭ����: ��push�µ�����
 *      ��ѭ����������ʱ��, �ȼ��ʣ��Ŀռ��Ƿ���Ա����������
 *      ���ݰ�, ������ܵĻ�, ���ȶ�ȡ����������ֱ��ʣ��ռ����
 *      ��Ÿ������ݵ�ʱ���ٽ��µ�����push��ѭ���ڴ滺�����С�
 *      ��pop���ݵ��ⲿ��buffer��ʱ��, �ȶ�ȡsizeof(size_t)���ȵ���
 *      ������֪���������ݰ��ĳ�����Ϣ, �ٸ��ݵ�֪�����ݳ�������
 *      ȡ�����������ݰ����ݡ�
 * 
 *      
 *      class CircularBuffer {
 *          sl::Lock*   l_; // ���̲߳�����
 *          sl::Buffer* b_; // �ڴ滺����������
 *              ...
 *      };
 * 
 *      @��CircularBuffer������ɹ�֮�󴴽���ѭ���ڴ�ռ�
 *          �ɹ�����true, ���򷵻�false
 *      bool create(sl::size_t bufLen = 1024);
 * 
 *      @��CircularBuffer����֮ǰ����release��Ա�������ͷŶ�Ӧ
 *          ��ѭ���������ڴ�ռ�
 *      void release(void);
 *
 *      @����clear��Ա�������CircularBuffer������ڴ�����, ��
 *          ��������������Ϣ��������Ϊ0, ͬʱ���ö�дָ���λ��
 *      void clear(void);
 * 
 *      @����push��Ա��������һ��������inLen�Լ�������Ϣ��
 *          inBuf��push��ѭ������������, ��ѭ����������������
 *          ��inLen��ֵ, Ȼ�����Ǵ�ŵ�inBuf�е�������Ϣ. ���
 *          û�д������ͷ���true, ���򷵻�false
 *      bool push(const sl::byte* inBuf, sl::size_t inLen);
 *
 *      @�ú����������صĺ�����������
 *      bool push(const sl::FrameBlock* inBlock);
 * 
 *      @����pop��Ա��������ȡһ��������֡���ݰ�. ��ѭ������
 *          �����ȶ�ȡsizeof(size_t)���ȵı�������֪���������
 *          ����֡�ĳ���, �ٸ��ݸó�������ȡ����������֡����
 *          FrameBlock������.
 *      @return<bool>: ��ѭ��������pop���ݳɹ�����true, ���򷵻�false
 *      bool pop(sl::FrameBlock* outBlock);
 */
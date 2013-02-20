/*
 * sl_log_doc.h - document for sl_log interfaces
 * 
 * Copyrights (c) 2011, 
 *      ASMlover. All rights reserved. 
 * 
 * created at 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * @��ϵ�еĽӿں�����Ҫ���ڴ�ӡ��ͬ�ȼ�����־��Ϣ, 
 *      ��Ҫʵ���˽���ͬ�ȼ�����־��Ϣ��ӡ����ͬ���ļ�
 *      ���������Ŀ�����ʾ�ĵط�(�����Ҫ�����ʵ��)
 * @��ϵ�к�������sl_log_debugx�ӿ�ΪĬ��ʹ�õ�log��־������Ϣ
 *      �Ĵ�ӡ, ����־�ļ������./sl_log/Ŀ¼��
 * 
 * 
 *      @��Ҫ������ʾ�������Ϣ, ��Ҫ�����ڳ����������
 *          ���±����ĵط�, ��ӡ����־��Ϣ�л���ʾ����ľ���
 *          errno����Ϣ
 *      @param [in] v<int>: ��������exit���õ�ֵ
 *      extern void sl_log_err(int v, const char* fmt, ...);
 * 
 *      @sl_log_warn�����ӿ���Ҫ������ʾ������Ϣ, ���������exit
 *          �������ж�(���)���������, ͬʱҲ����ʾ�����errno
 *          ���ַ�����Ϣ
 *      void sl_log_warn(const char* fmt, ...);
 * 
 *      @sl_log_errx�����ӿڵ����ú�sl_log_err����, Ψһ��ͬ����
 *          �ú������Ὣ�����errno��Ϣ��ʾ����
 *      void sl_log_errx(int v, const char* fmt, ...);
 * 
 *      @sl_log_warnx�����ӿڵ����ú�sl_log_warn��������, ��ͬ����
 *          �ú���������ʾ�����errno��Ϣ
 *      void sl_log_warnx(const char* fmt, ...);
 *
 *      @sl_log_msgx������Ҫ������ʾ��Ϣ��������ļ����������Ŀɹ�
 *          ��ʾ�ĵط�, ������ʾ�����������־��, ���ͳ�����ĵ���
 *          �����ǳ���ĵ����޹صĵط������Խ��������еĹؼ�������־
 *          ��ʾ��ָ���ĵط����ͻ��鿴
 *      void sl_log_msgx(const char* fmt, ...);
 * 
 *      @sl_log_debugx������Ҫ������ʾ����ĵ�����Ϣ, ����Ϣ��Ҫ�Ǹ�
 *          ������Ա�鿴���Ա���Գ������BUG��
 *          �ú���Ҳ�Ǹ�sl_log�ӿ�ϵ�е�Ĭ��ʹ�õĺ����ӿ�, ��ר�ŵ�
 *          ������sl_debug
 *      void sl_log_debugx(const char* fmt, ...);
 * 
 * 
 * ����: 
 *      sl_debug("xxxx �������� xxxx");
 *      ʹ�÷�����printf��������
 */
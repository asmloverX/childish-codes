/*
 * sl_log_doc.h - document for sl_log interfaces
 * 
 * Copyrights (c) 2011, 
 *      ASMlover. All rights reserved. 
 * 
 * created at 2011/01/12 12:06:50 by ASMlover.
 */
/*
 * @该系列的接口函数主要用于打印不同等级的日志信息, 
 *      主要实现了将不同等级的日志信息打印到不同的文件
 *      或者其他的可以显示的地方(这点需要具体的实现)
 * @该系列函数的中sl_log_debugx接口为默认使用的log日志调试信息
 *      的打印, 将日志文件输出到./sl_log/目录下
 * 
 * 
 *      @主要用于显示出错的信息, 主要运用于程序会出错可能
 *          导致崩溃的地方, 打印的日志信息中会显示出错的具体
 *          errno的信息
 *      @param [in] v<int>: 出错后调用exit设置的值
 *      extern void sl_log_err(int v, const char* fmt, ...);
 * 
 *      @sl_log_warn函数接口主要用于显示警告信息, 而不会调用exit
 *          函数来中断(或崩)程序的运行, 同时也会显示出错的errno
 *          的字符串信息
 *      void sl_log_warn(const char* fmt, ...);
 * 
 *      @sl_log_errx函数接口的作用和sl_log_err类似, 唯一不同的是
 *          该函数不会将出错的errno信息显示出来
 *      void sl_log_errx(int v, const char* fmt, ...);
 * 
 *      @sl_log_warnx函数接口的作用和sl_log_warn函数类似, 不同的是
 *          该函数不会显示出错的errno信息
 *      void sl_log_warnx(const char* fmt, ...);
 *
 *      @sl_log_msgx函数主要用于显示信息到具体的文件或者其他的可供
 *          显示的地方, 可以显示程序的运行日志等, 而和程序本身的调试
 *          或者是出错的调试无关的地方。可以将程序运行的关键运行日志
 *          显示到指定的地方供客户查看
 *      void sl_log_msgx(const char* fmt, ...);
 * 
 *      @sl_log_debugx函数主要用于显示程序的调试信息, 该信息主要是给
 *          开发人员查看的以便调试出程序的BUG等
 *          该函数也是该sl_log接口系列的默认使用的函数接口, 有专门的
 *          操作宏sl_debug
 *      void sl_log_debugx(const char* fmt, ...);
 * 
 * 
 * 例子: 
 *      sl_debug("xxxx 运行正常 xxxx");
 *      使用方法与printf函数类似
 */
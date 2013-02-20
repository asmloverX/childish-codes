/*
 * Copyright (c) 2011, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 * 
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#ifndef __SL_LOG_HEADER_H__
#define __SL_LOG_HEADER_H__

#ifndef _SL_LOG_
#define _SL_LOG_

#ifdef __cplusplus
extern {
#endif /* __cplusplus */


enum {
    SL_LOG_DEBUG = 0, 
    SL_LOG_MSG   = 1, 
    SL_LOG_WARN  = 2, 
    SL_LOG_ERR   = 3
};

extern void sl_log_err(int v, const char* fmt, ...);
extern void sl_log_warn(const char* fmt, ...);
extern void sl_log_errx(int v, const char* fmt, ...);
extern void sl_log_warnx(const char* fmt, ...);
extern void sl_log_msgx(const char* fmt, ...);
extern void sl_log_debugx(const char* fmt, ...);

#if _WIN32 || _WIN64
    #if _DEBUG
        #define sl_debug    sl_log_debugx
    #else
        #define sl_debug
    #endif
#else
    #ifndef NDEBUG
        #define sl_debug    sl_log_debugx
    #else
        #define sl_debug
    #endif
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* _SL_LOG_ */

#endif  /* __SL_LOG_HEADER_H__ */
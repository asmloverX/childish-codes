/*
 * Copyright (c) 2010, 
 *      ASMlover. All rights reserved.
 *
 *      http://blog.csdn.net/zfxfcx/
 *
 * Use, modification and distribution are subject to the 
 * "GNU GPL" at listed at <http://www.gnu.org/licenses/>. 
 */
#ifndef __CL_DEBUG_HEADER_H__
#define __CL_DEBUG_HEADER_H__

extern void 
dbgPrintA(const char* fname, const char* func, int line, const char* format, ...);
#if _WIN32 || _WIN64
#if _DEBUG
#define dbgPrint(_format, ...)  dbgPrintA(__FILE__, __FUNCTION__, __LINE__, _format, __VA_ARGS__)
#else
#define dbgPrint(_format, ...)
#endif
#else
#if NDEBUG
#define dbgPrint(_format, ...)
#else
#define dbgPrint(_format, ...)  dbgPrintA(__FILE__, __FUNCTION__, __LINE__, _format, __VA_ARGS__)
#endif
#endif

#endif  /* __CL_DEBUG_HEADER_H__ */
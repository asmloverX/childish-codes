// sl_network_test.h - sl_network testing interfaces
// 
// Copyright (c) 2011, 
//      CASMlover. All rights reserved.
// 
// created at 2011/01/05 11:15:18 by ASMlover.
#ifndef __SL_NETWORK_TEST_HEADER_H__
#define __SL_NETWORK_TEST_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#ifndef _SL_NETWORK_TEST_
#define _SL_NETWORK_TEST_

namespace sl_test {

#if _WIN32 || _WIN64
    #pragma warning(disable:4996)
#endif

#ifndef SAFEHANDLE_FREE
    #define SAFEHANDLE_FREE(h) { if (NULL != h) { CloseHandle(h); h = NULL; } }
#endif

    typedef unsigned char  uchar, byte;
    typedef unsigned short ushort, word_t;
    typedef unsigned int   uint, size_t;
    typedef unsigned long  ulong, dword_t;

    enum 
    {
        MSG_TYPE_STR = 0, 
        MSG_TYPE_BIN = 1
    };

    struct __msg_head 
    {
        ushort len_;
        word_t type_;
    };

    void sl_network_test(void);

    void sl_network_server(void);
    void sl_network_client(void);
    void sl_network_packet(void);
    void sl_network_packetmgr(void);
    void sl_network_capp(void);
    void sl_network_sapp(void);
    void sl_network_lock(void);
    void sl_network_buffer(void);
    void sl_network_frame(void);
    void sl_network_circularbuffer(void);
    void sl_network_version(void);

}

#endif  // _SL_NETWORK_TEST_

#endif  // __SL_NETWORK_TEST_HEADER_H__
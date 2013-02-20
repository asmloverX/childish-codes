// sl_network_test_packetmgr.cc - testing class sl::PacketMgr
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/07 09:23:34 by ASMlover.
#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include "../../inc/sl_packetmgr.h"
#include "../../inc/sl_netprotocol.h"

#include "sl_network_test.h"



namespace sl_test {

#ifndef SUBPACKAGE_DIR
    #define SUBPACKAGE_DIR  ("./SUBPACKAGE_DIR")
#endif
#ifndef COMBINATION_DIR
    #define COMBINATION_DIR ("./COMBINATION_DIR")
#endif


    static bool 
    __fullpacket_f(const byte* outBuf, ulong sz, void* args)
    {
        SYSTEMTIME st = {0};
        char fname[100] = {0};

        GetLocalTime(&st);
        if (NULL != outBuf && 0L != sz) {
            sprintf(fname, "%s/%.2d-%.2d-%.3d#%.8X.c", COMBINATION_DIR, 
                st.wMinute, st.wSecond, st.wMilliseconds, outBuf);
            fprintf(stdout, "begin to write file [%s], size = %ld\n", fname, sz);
            if (!PathFileExistsA(COMBINATION_DIR))
                CreateDirectoryA(COMBINATION_DIR, NULL);

            FILE* p = fopen(fname, "wb");
            fwrite(outBuf, sizeof(byte), (size_t)sz, p);
            fclose(p);
            fprintf(stdout, "write file [%s], size = %ld successed\n", fname, sz);

            return true;
        }
        return false;
    }

    static bool 
    __subpacket_f(const byte* dat, ushort sz, void* args)
    {
        sl::NetProtocol np = {0};
        sl::PacketMgr* pm = (sl::PacketMgr*)args;

        if (NULL != dat && 0 != sz && NULL != pm) {
            memcpy(&np, dat, sizeof(np));
            pm->combination(&np, dat + sizeof(np), __fullpacket_f, args);

            return true;
        }
        return false;
    }

    void 
    sl_network_packetmgr(void)
    {
        const char* fArray[] = {
            "sl_log.c", "sl_log.h", "sl_packet.cc", 
            "sl_packet.h", "sl_socket.cc", 
            "sl_socket.h", "Unix.pdf"
        };
        char fname[100] = {0};
        int count = sizeof(fArray) / sizeof(fArray[0]);

        sl::PacketMgr pm;
        pm.init();

        for (int i = 0; i < count; ++i) {
            sprintf(fname, "%s/%s", SUBPACKAGE_DIR, fArray[i]);
            
            FILE* p = fopen(fname, "rb");
            if (NULL != p) {
                fseek(p, 0, SEEK_END);
                long sz = ftell(p);
                fprintf(stdout, "\nbegin to get %s file, size = %ld ...\n", fname, sz);
                fseek(p, 0, SEEK_SET);

                byte* buf = (byte*)calloc(sz, sizeof(byte));
                fread(buf, sizeof(byte), sz, p);
                pm.subpackage(buf, sz, __subpacket_f, &pm);
                free(buf);

                fclose(p);
            }
        }
    }

}
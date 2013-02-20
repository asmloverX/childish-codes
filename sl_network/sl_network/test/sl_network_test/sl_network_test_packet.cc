// sl_network_test_packet.cc - testing class sl::Packet
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/06 08:48:23 by ASMlover.
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <hash_map>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include "../../inc/sl_netprotocol.h"
#include "../../inc/sl_packet.h"

#include "sl_network_test.h"



namespace sl_test {

#ifndef SUBPACKAGE_DIR
    #define SUBPACKAGE_DIR  ("./SUBPACKAGE_DIR")
#endif
#ifndef COMBINATION_DIR
    #define COMBINATION_DIR ("./COMBINATION_DIR")
#endif

    class PacketOper
    {
        stdext::hash_map<ushort, sl::Packet> pMap_;
    public:
        PacketOper(void) {}
        ~PacketOper(void) {}
    public:
        bool subpackageToCombination(ushort id, const char* fname)
        {
            byte*  buf = NULL;
            size_t sz  = 0L;
            std::vector<sl::SubPacket> spArray;

            FILE* p = fopen(fname, "rb");
            fseek(p, 0, SEEK_END);
            sz = ftell(p);
            fseek(p, 0, SEEK_SET);
            buf = (byte*)calloc(sz, sizeof(byte));
            fread(buf, sizeof(byte), sz, p);
            fclose(p);

            spArray.clear();
            subpackage(id, buf, sz, &spArray);
            pMap_[id].create(spArray[0].getCount());
            for (int i = 0; i < (int)spArray.size(); ++i)
                pMap_[id].addSubPacket(spArray[i]);

            if (NULL != buf)
            {
                free(buf);
                buf = NULL;
            }
            return true;

        }

        bool writeCombination(void)
        {
            stdext::hash_map<ushort, sl::Packet>::iterator i = pMap_.begin();
            char fname[100] = {0};

            if (!PathFileExistsA(COMBINATION_DIR))
                CreateDirectoryA(COMBINATION_DIR, NULL);
            for (; i != pMap_.end();) 
            {
                sprintf(fname, "%s/#%d.dat", COMBINATION_DIR, i->first);
                FILE* p = fopen(fname, "wb");

                //byte* buf = (byte*)calloc(i->second.getSize(), sizeof(byte));
                //i->second.getData(i->second.getSize(), buf);
                //fwrite(buf, i->second.getSize(), sizeof(byte), p);
                //free(buf);
                fwrite(i->second.getData(), i->second.getSize(), sizeof(byte), p);
                fclose(p);

                i->second.release();
                i = pMap_.erase(i);
            }

            return true;
        }

    private:
        bool subpackage(ushort id, const byte* dat, size_t len, std::vector<sl::SubPacket>* spArray)
        {
            sl::NetProtocol np = {0};
            ushort count = 0;

            count = (ushort)(len / SL_NETPROTO_PACKET_SZ);
            count = (ushort)(0 == len % SL_NETPROTO_PACKET_SZ ? count : count + 1);

            np.signature_ = SL_NETPROTO_SIGNATURE;
            np.version_.ver_ = SL_NETPROTO_VERSION;
            np.packSessionId_ = id;
            np.subPackCount_ = count;
            for (int i = 0; i < count; ++i)
            {
                np.subPackIndex_ = i + 1;
                np.subPacketsz_ = (i != count - 1 ? SL_NETPROTO_PACKET_SZ : len - (i * SL_NETPROTO_PACKET_SZ));

                sl::SubPacket sp(&np, dat + i * SL_NETPROTO_PACKET_SZ);
                spArray->push_back(sp);
            }
            return true;
        }
    };

    void 
    sl_network_packet()
    {
        const char* fArray[] = {
            "sl_log.c", "sl_packet.cc", "sl_socket.cc", "Unix.pdf"
        };
        char buf[100] = {0};
        int count = sizeof(fArray) / sizeof(fArray[0]);

        PacketOper p;

        srand((uint)time(0));
        for (int i = 0; i < count; ++i)
        {
            sprintf(buf, "%s/%s", SUBPACKAGE_DIR, fArray[i]);
            p.subpackageToCombination((ushort)rand() % 100, buf);
        }

        p.writeCombination();
    }

}
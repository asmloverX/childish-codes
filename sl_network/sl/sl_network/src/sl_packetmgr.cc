// sl_packetmgr.cc - Implementation of PacketMgr class for 
//      managing read/write data.
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/07 09:00:30 by ASMlover.
#include <stdio.h>
#include <hash_map>
#include <string.h>
#include "../inc/sl_netprotocol.h"
#include "../inc/sl_packet.h"

#include "../inc/sl_packetmgr.h"


namespace sl {

#ifndef SAFEOBJ_FREE
    #define SAFEOBJ_FREE(_obj)  { if (NULL != (_obj)) { delete (_obj); (_obj) = NULL; } }
#endif

    PacketMgr::PacketMgr(void)
        : packMap_(NULL), sessionId_(0) 
    {
    }

    PacketMgr::~PacketMgr(void)
    {
    }

    bool PacketMgr::init(void)
    {
        packMap_ = new stdext::hash_map<ushort, Packet>;

        return (NULL != packMap_);
    }

    void PacketMgr::uninit(void)
    {
        if (NULL != packMap_) 
        {
            stdext::hash_map<ushort, Packet>* packMap = 
                (stdext::hash_map<ushort, Packet>*)packMap_;
            stdext::hash_map<ushort, Packet>::iterator i;

            for (i = packMap->begin(); i != packMap->end();)
            {
                i->second.release();
                i = packMap->erase(i);
            }
            SAFEOBJ_FREE(packMap);
            packMap_ = NULL;
        }
        sessionId_ = 0;
    }

    bool PacketMgr::subpackage(
        const byte* data, 
        const ulong& size,  
        bool (*spFN)(const byte* outBuf, ushort size, void* args), 
        void* args
        )
    {
        byte buf[1024] = {0};
        struct NetProtocol np = {0};

        if (NULL != data && size > 0L) 
        {
            ushort count = (ushort)(size / SL_NETPROTO_PACKET_SZ);
            count = (0 == size % SL_NETPROTO_PACKET_SZ ? count : count + 1);

            np.signature_     = SL_NETPROTO_SIGNATURE;
            np.version_.ver_  = SL_NETPROTO_VERSION;
            np.packSessionId_ = sessionId_++;
            np.subPackCount_  = count;

            for (int i = 0; i < count; ++i)
            {
                np.subPackIndex_ = i + 1;
                np.subPacketsz_  = (ushort)(i == count - 1 ? 
                    size - (i * SL_NETPROTO_PACKET_SZ) : SL_NETPROTO_PACKET_SZ);

                memset(buf, 0, sizeof(buf));
                memcpy(buf, &np, sizeof(np));
                memcpy(buf + sizeof(np), data + i * SL_NETPROTO_PACKET_SZ, np.subPacketsz_);

                if (NULL != spFN)
                    spFN(buf, np.subPacketsz_ + sizeof(np), args);
            }
            return true;
        }
        return false;
    }

    bool PacketMgr::combination(
        const NetProtocol* np, 
        const byte* data,  
        bool (*fpFN)(const byte* outBuf, ulong size, void* args), 
        void* args
        )
    {
        if (NULL != packMap_ && NULL != np && NULL != data) 
        {
            stdext::hash_map<ushort, Packet>* packMap = 
                (stdext::hash_map<ushort, Packet>*)packMap_;
            SubPacket sp(np, data);

            if (np->subPackCount_ > 1)
            {
                if (packMap->end() == packMap->find(sp.getSessionId())) 
                    addNewPacketDataToMap(sp);
                else 
                    addExistsPacketDataToMap(sp, fpFN, args);
            } 
            else 
            {
                if (NULL != fpFN)
                    fpFN(data, np->subPacketsz_, args);
            }
            return true;
        }
        return false;
    }

    bool PacketMgr::addNewPacketDataToMap(const SubPacket& sp)
    {
        if (NULL != packMap_)
        {
            stdext::hash_map<ushort, Packet>* packMap = 
                (stdext::hash_map<ushort, Packet>*)packMap_;
            Packet p;

            p.create(sp.getCount());
            p.addSubPacket(sp);

            (*packMap)[sp.getSessionId()] = p;

            return true;
        }
        return false;
    }

    bool PacketMgr::addExistsPacketDataToMap(
        const SubPacket& sp, 
        bool (*fpFN)(const byte* outBuf, ulong size, void* args), 
        void* args
        )
    {
        if (NULL != packMap_)
        {
            stdext::hash_map<ushort, Packet>* packMap = 
                (stdext::hash_map<ushort, Packet>*)packMap_;

            (*packMap)[sp.getSessionId()].addSubPacket(sp);
            if ((*packMap)[sp.getSessionId()].getSize() > 
                (ulong)((sp.getCount() - 1) * SL_NETPROTO_PACKET_SZ))
            {
                if (NULL != fpFN)
                {
                    fpFN((*packMap)[sp.getSessionId()].getData(), 
                        (*packMap)[sp.getSessionId()].getSize(), args);
                }
                (*packMap)[sp.getSessionId()].release();
                packMap->erase(sp.getSessionId());
            }
            return true;
        }
        return false;
    }

}
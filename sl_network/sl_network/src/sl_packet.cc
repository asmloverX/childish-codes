// sl_packet.cc - Implementation of SubPacket/Packet class.
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/05 15:46:17 by ASMlover.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/sl_netprotocol.h"

#include "../inc/sl_packet.h"


namespace sl {

    SubPacket::SubPacket(void)
    {
        memset(this, 0, sizeof(*this));
    }

    SubPacket::SubPacket(const NetProtocol* np, const byte* data)
    {
        memset(this, 0, sizeof(*this));
        setValue(np, data);
    }

    SubPacket::SubPacket(ushort sessionId, ushort count, 
        ushort index, ushort size, const byte* data)
    {
        memset(this, 0, sizeof(*this));
        setValue(sessionId, count, index, size, data);
    }

    SubPacket::~SubPacket(void)
    {
    }

    bool SubPacket::setValue(const NetProtocol* np, const byte* data)
    {
        if (NULL != np && NULL != data)
        {
            sessionId_ = np->packSessionId_;
            count_     = np->subPackCount_;
            index_     = np->subPackIndex_;
            size_      = np->subPacketsz_;
            data_      = (byte*)data;
            
            return true;
        }
        return false;
    }

    bool SubPacket::setValue(ushort sessionId, ushort count, 
        ushort index, ushort size, const byte* data)
    {
        if (NULL != data)
        {
            sessionId_ = sessionId;
            count_     = count;
            index_     = index;
            size_      = size;
            data_      = (byte*)data;

            return true;
        }
        return false;
    }

    const ushort SubPacket::getSessionId(void) const
    {
        return sessionId_;
    }

    const ushort SubPacket::getCount(void) const
    {
        return count_;
    }

    const ushort SubPacket::getIndex(void) const
    {
        return index_;
    }

    const ushort SubPacket::getSize(void) const
    {
        return size_;
    }

    const byte* SubPacket::getData(void) const
    {
        return (const byte*)data_;
    }


    Packet::Packet(void)
    {
        memset(this, 0, sizeof(*this));
    }

    Packet::~Packet(void)
    {
    }

    bool Packet::create(ushort subPackCount)
    {
        if (subPackCount > 0)
        {
            if (NULL != data_)
                return true;
            data_ = (byte*)calloc(subPackCount, SL_NETPROTO_PACKET_SZ);
            return (NULL != data_);
        }
        return false;
    }

    void Packet::release(void)
    {
        if (NULL != data_)
        {
            free(data_);
            data_ = NULL;
        }
        size_ = 0L;
        sessionId_ = 0;
    }

    const ushort Packet::getSessionId(void) const
    {
        return sessionId_;
    }

    const ulong Packet::getSize(void) const 
    {
        return size_;
    }

    bool Packet::addSubPacket(const SubPacket& sp)
    {
        if (NULL != data_ && sp.getSize() > 0)
        {
            sessionId_ = sp.getSessionId();
            memcpy(data_ + (sp.getIndex() - 1) * SL_NETPROTO_PACKET_SZ, 
                sp.getData(), sp.getSize());
            size_ += sp.getSize();

            return true;
        }
        return false;
    }

    bool Packet::getData(ulong size, byte* data)
    {
        if (NULL != data_ && NULL != data)
        {
            memcpy(data, data_, (size >= size_ ? size_ : size));
            return true;
        }
        return false;
    }

    const byte* Packet::getData(void) const
    {
        return data_;
    }

}
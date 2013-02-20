// sl_packet.h - SubPacket class and Packet class.
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/05 14:46:53 by ASMlover.
#ifndef __SL_PACKET_HEADER_H__
#define __SL_PACKET_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#ifndef _SL_PACKET_
#define _SL_PACKET_

namespace sl {

    typedef unsigned char  byte;
    typedef unsigned short ushort;
    typedef unsigned long  ulong;

    struct NetProtocol;
    class SubPacket 
    {
        ushort sessionId_;  // session ID of this SubPacket
        ushort count_;      // count of SubPacket in one Packet
        ushort index_;      // index of SubPacket in one Packet
        ushort size_;       // size of this SubPacket
        byte*  data_;       // data of this SubPacket
    public:
        SubPacket(void);
        SubPacket(const struct NetProtocol* np, const byte* data);
        SubPacket(ushort sessionId, ushort count, 
          ushort index, ushort size, const byte* data);
        virtual ~SubPacket(void);
    public:
        bool setValue(const struct NetProtocol* np, const byte* data);
        bool setValue(ushort sessionId, ushort count, 
            ushort index, ushort size, const byte* data);

        const ushort getSessionId(void) const;
        const ushort getCount(void) const;
        const ushort getIndex(void) const;
        const ushort getSize(void) const;
        const byte* getData(void) const;
    };

    // Class Packet
    // 
    // example:
    //      sl::Packet p;
    //  1> create a Packet instance
    //      p.create(SubPacketCountsOfThisPacket);
    //  2> add SubPacket to Packet or get data from Packet
    //      SubPacket sp(NetProtocolHead, SubPacketData);
    //      p.add(sp);
    //          ... 
    //      byte buf[100] = {0};
    //      p.getData(sizeof(buf), (byte*)buf);
    //     or
    //      memcpy(buf, p.getData(), sizeof(buf));
    //  3> release this Packet instance after use
    //      p.release();
    class Packet 
    {
        ushort sessionId_;  // session ID of this Packet
        ulong  size_;       // size of this full Packet
        byte*  data_;       // data of this full Packet
    public:
        Packet(void);
        ~Packet(void);
    public:
        bool create(ushort subPackCount = 1);
        void release(void);

        const ushort getSessionId(void) const;
        const ulong getSize(void) const;
        bool addSubPacket(const SubPacket& sp);
        bool getData(ulong size, byte* data);
        const byte* getData(void) const;
    };

}

#endif  // _SL_PACKET_

#endif  // __SL_PACKET_HEADER_H__
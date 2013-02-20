//sl_packetmgr.h - PacketMgr class for managing read/write data.
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/07 08:31:07 by ASMlover.
#ifndef __SL_PACKETMGR_HEADER_H__
#define __SL_PACKETMGR_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#ifndef _SL_PACKETMGR_
#define _SL_PACKETMGR_

namespace sl {

    typedef unsigned char  byte;
    typedef unsigned short ushort;
    typedef unsigned long  ulong;

    // class PacketMgr
    // 
    // example:
    //      PacketMgr pm;
    //  1> initialize the class PacketMgr instance
    //      pm.init();
    //  2> subcontracting full packet data or grouping subpakcets
    //      pm.subpackage( ... );
    //      pm.combination( ... );
    //  3> uninitialize the instance of class PacketMgr after use
    //      pm.uninit();
    struct NetProtocol;
    class SubPacket;
    class PacketMgr 
    {
        void* packMap_;     // SubPacket hash_map list
        ushort sessionId_;  // session ID for full Packet
    public:
        PacketMgr(void);
        ~PacketMgr(void);
    public:
        bool init(void);
        void uninit(void);

        // Sub Packet Functional - spFN
        // 
        // Subcontracting operations for effective full packet data.
        // 
        // example:
        //  Suppose SubPacket operation function is: 
        //      static __write_data(const byte* outBuf_, const ushort& sz_, void* args_)
        //      {
        //              ... write data to other side of network ...
        //      }
        //  
        //  PacketMgrObj.subpackage(fullData, fullDataSize, __write_data, this); 
        //  this operation can send fullData to other side of network by N SubPacket.
        bool subpackage(
            const byte* data, 
            const ulong& size, 
            bool (*spFN)(const byte* outBuf, ushort size, void* args), 
            void* args
            );

        // Full Packet Functional - fpFN
        // 
        // example:
        //  Suppose full packet operation function is:
        //      static __full_packet_show(
        //          const byte* outBuf_, const ulong& size_, void* args_)
        //      {
        //              ... show full packet data ...
        //      }
        // 
        // PacketMgrObj.combination(NetProtocolHead, 
        //      SubPacketData, __full_packet_show, this);
        // this function can show full packet data when all subpackets grouped 
        // to full packet data.
        bool combination(
            const NetProtocol* np, 
            const byte* data,  
            bool (*fpFN)(const byte* outBuf, ulong size, void* args), 
            void* args
            );
    private:
        bool addNewPacketDataToMap(const SubPacket& sp);
        // Full Packet Functional - fpFN
        bool addExistsPacketDataToMap(
            const SubPacket& sp, 
            bool (*fpFN)(const byte* outBuf, ulong size, void* args), 
            void* args
            );
    };

}

#endif  // _SL_PACKETMGR_

#endif  // __SL_PACKETMGR_HEADER_H__
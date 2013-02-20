// sl_netprotocol.h - Net transfer protocol
// 
// Copyright (c) 2011, 
//      ASMlover. All rigths reserved.
// 
// created at 2011/01/05 15:18:26 by ASMlover.
#ifndef __SL_NET_PROTOCOL_HEADER_H__
#define __SL_NET_PROTOCOL_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#ifndef _SL_NETPROTOCOL_
#define _SL_NETPROTOCOL_

namespace sl {

#define SL_NETPROTO_PORT_DEF    (20093)     // default network port
#define SL_NETPROTO_SIGNATURE   (0xA9EE)    // network protocol signature
#define SL_NETPROTO_VERSION     (0x0100)    // network protocol version
#define SL_NETPROTO_PACKET_SZ   (768)       // size of network protocol packet

    typedef unsigned char  byte;
    typedef unsigned short ushort;

#pragma pack(push, 1)
    // struct NetProtocol
    // 
    //                  memory module for full packet
    //  |------------------------|----------------------------------------|
    //      NetProtocol head                  full packet data
    // 
    // memory module for one subpacket of full packet
    //  |------------------------|--------|
    //      NetProtocol head       subpacket data
    //
    //  (full packet data) = N * (subpacket data)
    //  (one subpacket data) = SL_NETPROTO_PACKET_SZ bytes
    //
    //  If the (full packet data) too long, it will be divided 
    //  into N subpacket parts. All (subpacket data) identified 
    //  by the same session ID of (full packet data).
    struct NetProtocol
    {
        ushort signature_;
        union 
        {
            ushort ver_;
            struct { byte low_, high_; } sVer_;
        } version_;
        ushort packSessionId_;
        ushort subPackCount_;
        ushort subPackIndex_;
        ushort subPacketsz_;
    };
#pragma pack(pop)

}

#endif  // _SL_NETPROTOCOL_

#endif  // __SL_NET_PROTOCOL_HEADER_H__
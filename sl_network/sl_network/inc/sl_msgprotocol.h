// sl_msgprotocol.h - message protocol
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/03 17:36:24 by ASMlover.
#ifndef __SL_MSGPROTOCOL_HEADER_H__
#define __SL_MSGPROTOCOL_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#ifndef _SL_MSGPROTOCOL_
#define _SL_MSGPROTOCOL_

#define SLMP_CHANNEL_INVALID        (0xFFFF)

#define SLMP_CMDMAIN_BASIC          (0x0001)
#define SLMP_CMDBASIC_LOGIN         (0x0001)    // 登录
#define SLMP_CMDBASIC_LOGOFF        (0x0002)    // 注销
#define SLMP_CMDBASIC_GETCTL        (0x0003)    // 获取控制权

// 服务内容查询(客户端向服务端查询可提供的服务)
#define SLMP_CMDMAIN_QUERY          (0x0002)
#define SLMP_CMDQUERY_CHANNELNUM    (0x0001)    // 信道数
#define SLMP_CMDQUERY_AUDIO         (0x0002)    // 是否支持声音播放
#define SLMP_CMDQUERY_BUSSINESS     (0x0003)    // 支持的业务
#define SLMP_CMDQUERY_DEVARGS       (0x0004)    // 设备的参数

// 任务相关
#define SLMP_CMDMAIN_TASK           (0x0003)
#define SLMP_CMDTASK_NULL           (0x0000)    // 空任务, 对设备直接进行参数修改时使用, 设备立即生效
#define SLMP_CMDTASK_INSTANT        (0x0001)    // 紧急任务, 一直执行, 直到中断
#define SLMP_CMDTASK_SCHED          (0x0002)    // 定时任务, 计划任务

// 服务器广播的实时数据
#define SLMP_CMDMAIN_RTDATA         (0x0004)
#define SLMP_CMDRTDATA_DEVICE       (0x0001)    // 来自设备的数据信息
#define SLMP_CMDRTDATA_AUDIO        (0x0002)    // 音频数据信息


typedef unsigned short   ushort, word_t;
typedef unsigned long    ulong, dword_t;
typedef unsigned __int64 uint64, ulonglong;

#pragma pack(push, 1)
// struct SL_MsgHead
//
//             memory module for one packet
// |-------------------------|--------------------------|
//         SL_MsgHead                packet data
// 
// One full packet message must contain a SL_MsgHead.
struct SL_MsgHead 
{
    ushort cbsize_; // size of struct SL_MsgHead
    union 
    {
        dword_t dw_;
        struct { word_t main_, sub_; } w_;
    } cmd_;
    ushort channelId_;

    // 保留参数
    ulong  param1_;
    ulong  param2_;
    ulong  param3_;
    uint64 args1_;
    uint64 args2_;
    uint64 args3_;
};
#pragma pack(pop)


#endif  // _SL_MSGPROTOCOL_

#endif  // __SL_MSGPROTOCOL_HEADER_H__
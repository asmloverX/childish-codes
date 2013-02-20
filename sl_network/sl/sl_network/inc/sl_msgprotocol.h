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
#define SLMP_CMDBASIC_LOGIN         (0x0001)    // ��¼
#define SLMP_CMDBASIC_LOGOFF        (0x0002)    // ע��
#define SLMP_CMDBASIC_GETCTL        (0x0003)    // ��ȡ����Ȩ

// �������ݲ�ѯ(�ͻ��������˲�ѯ���ṩ�ķ���)
#define SLMP_CMDMAIN_QUERY          (0x0002)
#define SLMP_CMDQUERY_CHANNELNUM    (0x0001)    // �ŵ���
#define SLMP_CMDQUERY_AUDIO         (0x0002)    // �Ƿ�֧����������
#define SLMP_CMDQUERY_BUSSINESS     (0x0003)    // ֧�ֵ�ҵ��
#define SLMP_CMDQUERY_DEVARGS       (0x0004)    // �豸�Ĳ���

// �������
#define SLMP_CMDMAIN_TASK           (0x0003)
#define SLMP_CMDTASK_NULL           (0x0000)    // ������, ���豸ֱ�ӽ��в����޸�ʱʹ��, �豸������Ч
#define SLMP_CMDTASK_INSTANT        (0x0001)    // ��������, һֱִ��, ֱ���ж�
#define SLMP_CMDTASK_SCHED          (0x0002)    // ��ʱ����, �ƻ�����

// �������㲥��ʵʱ����
#define SLMP_CMDMAIN_RTDATA         (0x0004)
#define SLMP_CMDRTDATA_DEVICE       (0x0001)    // �����豸��������Ϣ
#define SLMP_CMDRTDATA_AUDIO        (0x0002)    // ��Ƶ������Ϣ


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

    // ��������
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
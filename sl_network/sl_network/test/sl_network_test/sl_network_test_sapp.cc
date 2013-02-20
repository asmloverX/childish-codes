// sl_network_test_sapp.cc - testing sl_network with server
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/08 14:39:39 by ASMlover.
#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#include <hash_map>
#pragma comment(lib, "shlwapi.lib")
#include "../../inc/sl_netprotocol.h"
#include "../../inc/sl_msgprotocol.h"
#include "../../inc/sl_socket.h"
#include "../../inc/sl_dispatch.h"
#include "../../inc/sl_packetmgr.h"

#include "sl_network_test.h"


namespace sl_test {

#define PACKET_SZ_DEFAULT   (1024)

    typedef unsigned int uint;

    struct ClientInfo 
    {
        sl::SocketHandler sh_;
        sl::NetProtocol   np_;
        sl::PacketMgr     pm_;
        bool              hasNp_;

        ClientInfo(void)
            : hasNp_(false)
        {
            memset(&np_, 0, sizeof(np_));
        }

        static bool fullpacket_f(const byte* outBuf, ulong size, void* args)
        {
            ClientInfo* self = (ClientInfo*)args;
            struct SL_MsgHead h = {0};

            if (NULL != self && NULL != outBuf) 
            {
                memcpy(&h, outBuf, sizeof(h));
                return self->solveBusiness(h, outBuf + sizeof(h), size - sizeof(h));
            }
            return false;
        }
    private:
        static bool subpackage_s(const byte* inBuf, ushort sz, void* args)
        {
            ClientInfo* self = (ClientInfo*)args;

            return (NULL != self ? self->sh_.write((char*)inBuf, sz) : false);
        }

        bool sendMsg(const byte* data, ulong size)
        {
            return pm_.subpackage(data, size, &ClientInfo::subpackage_s, this);
        }

        void showMsgHead(const struct SL_MsgHead& h)
        {
            fprintf(stdout, 
                "This client SHandler = %d\n"
                "SL_MsgHead->channelId_ = %d\n"
                "SL_MsgHead->cmd_.w_.main_ = %.4X\n"
                "SL_MsgHead->cmd_.w_.sub_  = %.4x\n"
                "SL_MsgHead->cbsize_ = %d\n", 
                sh_.getSHandler(), h.channelId_, 
                h.cmd_.w_.main_, h.cmd_.w_.sub_, h.cbsize_);
        }

        bool solveBusiness(const struct SL_MsgHead& h, const byte* dat, ulong sz)
        {
            if (SLMP_CMDMAIN_BASIC == h.cmd_.w_.main_)
            {
                showMsgHead(h);
                switch (h.cmd_.w_.sub_) 
                {
                case SLMP_CMDBASIC_LOGIN:
                    solveLogin();
                    break;
                case SLMP_CMDBASIC_GETCTL:
                    solveGetCtl(dat, sz);
                    break;
                }
                return true;
            }
            return false;
        }

        bool solveLogin(void)
        {
            struct SL_MsgHead h = {0};

            h.cbsize_ = sizeof(h);
            h.channelId_ = SLMP_CHANNEL_INVALID;
            h.cmd_.w_.main_ = SLMP_CMDMAIN_BASIC;
            h.cmd_.w_.sub_  = SLMP_CMDBASIC_LOGIN;

            return sendMsg((byte*)&h, sizeof(h));
        }

        bool solveGetCtl(const byte* data, ulong size)
        {
            struct SL_MsgHead h = {0};
            byte*  buf = {0};

            h.cbsize_ = sizeof(h);
            h.channelId_ = SLMP_CHANNEL_INVALID;
            h.cmd_.w_.main_ = SLMP_CMDMAIN_BASIC;
            h.cmd_.w_.sub_  = SLMP_CMDBASIC_GETCTL;

            buf = (byte*)calloc(size + sizeof(h), sizeof(byte));
            memcpy(buf, &h, sizeof(h));
            memcpy(buf + sizeof(h), data, size);

            SYSTEMTIME bst = {0}, est = {0};
            GetLocalTime(&bst);
            bool ret = sendMsg(buf, size + sizeof(h));
            GetLocalTime(&est);
            fprintf(stdout, 
                "<<<<<<<<<<<<< Begin to send time[%.2d:%.2d:%.2d:%.3d] >>>>>>>>>>>>>>>>\n"
                "<<<<<<<<<<<<< End   to send time[%.2d:%.2d:%.2d:%.3d] >>>>>>>>>>>>>>>>\n", 
                bst.wHour, bst.wMinute, bst.wSecond, bst.wMilliseconds, 
                est.wHour, est.wMinute, est.wSecond, est.wMilliseconds);

            if (NULL != buf) 
            {
                free(buf);
                buf = NULL;
            }
            return ret;
        }

    };
    class SHandlerSApp :
        public sl::SocketHandler 
    {
        stdext::hash_map<uint, ClientInfo> cltMap_;
    public:
        SHandlerSApp(void) 
        {
            cltMap_.clear();
        }

        ~SHandlerSApp(void)
        {
        }
    public:
        void eventAccept(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_ACCEPT ... [Server]\n");

            sl::SocketHandler sh;

            if (acceptFrom(NULL, &sh))
            {
                sh.selEvent(FD_READ | FD_WRITE | FD_CLOSE);

                ClientInfo ci;
                ci.sh_ = sh;
                ci.hasNp_ = false;
                ci.pm_.init();
                cltMap_[sh.getSHandler()] = ci;
            }
        }

        void eventRead(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_READ ... [Server]\n");

            uint id = s->getSHandler();
            if (!cltMap_[id].hasNp_)
            {
                memset(&cltMap_[id].np_, 0, sizeof(sl::NetProtocol));
                s->read(sizeof(sl::NetProtocol), (char*)&cltMap_[id].np_);
                cltMap_[id].hasNp_ = true;
            }
            else 
            {
                byte buf[PACKET_SZ_DEFAULT] = {0};
                s->read(cltMap_[id].np_.subPacketsz_, (char*)buf);
                cltMap_[id].pm_.combination(&cltMap_[id].np_, buf, &ClientInfo::fullpacket_f, &cltMap_[id]);

                cltMap_[id].hasNp_ = false;
            }
        }

        void eventWrite(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_WRITE ... [Server]\n");
        }

        void eventClose(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_CLOSE ... [Server]\n");
            if (getSHandler() == s->getSHandler())
            {
                closeAllClients();
                close();
            }
            else 
            {
                cltMap_[s->getSHandler()].sh_.close();
                cltMap_[s->getSHandler()].pm_.uninit();
                cltMap_.erase(s->getSHandler());
            }
        }
    private:
        void closeAllClients(void)
        {
            stdext::hash_map<uint, ClientInfo>::iterator i;

            for (i = cltMap_.begin(); i != cltMap_.end();) 
            {
                i->second.sh_.close();
                i->second.pm_.uninit();
                i = cltMap_.erase(i);
            }
        }
    };

    void 
    sl_network_sapp(void)
    {
        sl::Dispatch_S ds;
        SHandlerSApp   shs;

        ds.setSHandler(&shs);
        ds.begin(0, SL_NETPROTO_PORT_DEF);

        while (true)
            Sleep(100);
        ds.end();
    }

}
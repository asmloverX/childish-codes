// sl_network_test_capp.cc - testing sl_network with client
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/08 10:31:29 by ASMlover.
#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include "../../inc/sl_netprotocol.h"
#include "../../inc/sl_socket.h"
#include "../../inc/sl_dispatch.h"
#include "../../inc/sl_packetmgr.h"
#include "../../inc/sl_msgprotocol.h"

#include "sl_network_test.h"


namespace sl_test {

#define PACKET_SZ_DEFAULT   (1024)

//#define SEND_PACKAGE_BY_THREAD

    class SHandlerCApp
        : public sl::SocketHandler
    {
        sl::NetProtocol np_;
        bool            hasNp_;
        sl::PacketMgr   pm_;

        bool            isConnected_;
        bool            isLogined_;
    public:
        SHandlerCApp(void)
            : hasNp_(false), isConnected_(false), isLogined_(false)
        {
            memset(&np_, 0, sizeof(np_));
            pm_.init();
        }

        ~SHandlerCApp(void)
        {
            hasNp_ = false;
            pm_.uninit();
            isConnected_ = false;
            isLogined_ = false;
        }
    public:
        const bool getConnected(void) const 
        {
            return isConnected_;
        }

        bool loginToServer(void)
        {
            struct SL_MsgHead h = {0};

            h.cbsize_ = sizeof(h);
            h.cmd_.w_.main_ = SLMP_CMDMAIN_BASIC;
            h.cmd_.w_.sub_  = SLMP_CMDBASIC_LOGIN;
            h.channelId_ = SLMP_CHANNEL_INVALID;

            return sendMsg((char*)&h, sizeof(h));
        }

        bool sendMsg(const char* inBuf, int len)
        {
            return pm_.subpackage((byte*)inBuf, (ulong)len, &SHandlerCApp::subpackage_s, this);
        }
    public:
        void eventConnect(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_CONNECT ... [Client]\n");
            isConnected_ = true;
        }

        void eventRead(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_READ ... [Client]\n");
            if (!hasNp_) 
            {
                memset(&np_, 0, sizeof(np_));
                read(sizeof(np_), (char*)&np_);
                hasNp_ = true;
            } 
            else 
            {
                char buf[PACKET_SZ_DEFAULT] = {0};
                read(np_.subPacketsz_, buf);
                pm_.combination(&np_, (byte*)buf, &SHandlerCApp::fullpacket_f, this);

                hasNp_ = false;
            }
        }

        void eventWrite(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_WRITE ... [Client]\n");
        }

        void eventClose(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_CLOSE ... [Client]\n");
            close();
        }
    private:
        struct ThreadArgs
        {
            byte*  data_;
            ushort size_;
            SHandlerCApp* this_;
        };
        static DWORD WINAPI sThread(void* args)
        {
            ThreadArgs ta = {0};
            memcpy(&ta, args, sizeof(ta));

            if (NULL != ta.this_)
                ta.this_->write((char*)ta.data_, ta.size_);

            return 0;
        }

        static bool subpackage_s(const byte* inBuf, ushort size, void* args)
        {
#ifdef SEND_PACKAGE_BY_THREAD
            ThreadArgs ta = { (byte*)inBuf, size, (SHandlerCApp*)args };
            CloseHandle(CreateThread(NULL, 0, sThread, &ta, 0, NULL));
            Sleep(10);

            return true;
#else
            SHandlerCApp* self = (SHandlerCApp*)args;
            return (NULL != self ? self->write((char*)inBuf, (int)size) : false);
#endif
        }

        static bool fullpacket_f(const byte* outBuf, ulong size, void* args)
        {
            SHandlerCApp* self = (SHandlerCApp*)args;
            struct SL_MsgHead h = {0};

            if (NULL != self) 
            {
                memcpy(&h, outBuf, sizeof(h));
                return self->solveBusiness(h, outBuf + sizeof(h), size - sizeof(h));
            }
            return false;
        }

        bool solveBusiness(const struct SL_MsgHead& h, const byte* dat, ulong sz)
        {
            if (SLMP_CMDMAIN_BASIC == h.cmd_.w_.main_)
            {
                switch (h.cmd_.w_.sub_)
                {
                case SLMP_CMDBASIC_LOGIN:
                    businessLogin(h);
                    break;
                case SLMP_CMDBASIC_LOGOFF:
                    break;
                case SLMP_CMDBASIC_GETCTL:
                    businessGetCtl(h, dat, sz);
                    break;
                }
                return true;
            }
            return false;
        }

        bool sendFile(void)
        {
            FILE* p   = fopen("./SUBPACKAGE_DIR/Unix.pdf", "rb");
            char* buf = NULL;
            long  sz  = 0;
            bool  ret = false;
            struct SL_MsgHead h = {0};

            fseek(p, 0, SEEK_END);
            sz = ftell(p);
            fseek(p, 0, SEEK_SET);

            buf = (char*)calloc(sz + sizeof(h), sizeof(char));
            h.cbsize_ = sizeof(h);
            h.channelId_ = SLMP_CHANNEL_INVALID;
            h.cmd_.w_.main_ = SLMP_CMDMAIN_BASIC;
            h.cmd_.w_.sub_  = SLMP_CMDBASIC_GETCTL;
            memcpy(buf, &h, sizeof(h));
            fread(buf + sizeof(h), sizeof(char), sz, p);
            fclose(p);

            SYSTEMTIME bst = {0}, est = {0};
            GetLocalTime(&bst);
            ret = sendMsg(buf, sz + sizeof(h));
            GetLocalTime(&est);
            fprintf(stdout, 
                "<<<<<<<<<<<<< Begin to send time[%.2d:%.2d:%.2d:%.3d] >>>>>>>>>>>>>>>>\n"
                "<<<<<<<<<<<<< End   to send time[%.2d:%.2d:%.2d:%.3d] >>>>>>>>>>>>>>>>\n", 
                bst.wHour, bst.wMinute, bst.wSecond, bst.wMilliseconds, 
                est.wHour, est.wMinute, est.wSecond, est.wMilliseconds);

            free(buf);
            return ret;
        }

        void showMsgHead(const struct SL_MsgHead& h)
        {
            fprintf(stdout,
                "SL_MsgHead.channelId_ = %d\n"
                "SL_MsgHead.cmd_.w_.main_ = %.4X\n"
                "SL_MsgHead.cmd_.w_.sub_  = %.4X\n"
                "SL_MsgHead.cbsize_ = %d\n", 
                h.channelId_, h.cmd_.w_.main_, h.cmd_.w_.sub_, h.cbsize_);
        }

        bool businessLogin(const struct SL_MsgHead& h)
        {
            showMsgHead(h);
            isLogined_ = true;
            fprintf(stdout, "Login to server success ... \n");

            sendFile();

            return false;
        }

        bool businessGetCtl(const struct SL_MsgHead& h, const byte* dat, ulong sz)
        {
            showMsgHead(h);

            if (!PathFileExistsA("./getctl"))
                CreateDirectoryA("./getctl", NULL);

            SYSTEMTIME st = {0};
            char fname[100] = {0};
            GetLocalTime(&st);
            sprintf(fname, "./getctl/getctl#%.2d-%.2d-%.3d#%.8X.c", 
                st.wMinute, st.wSecond, st.wMilliseconds, dat);
            FILE* p = fopen(fname, "wb");
            
            if (NULL != p) 
            {
                fwrite(dat, sizeof(byte), sz, p);
                fclose(p);

                return sendFile();
            }
            return false;
        }
    };

    void 
    sl_network_capp(void)
    {
        SHandlerCApp   shc;
        sl::Dispatch_C dc;

        dc.setSHandler(&shc);
        dc.begin("127.0.0.1", SL_NETPROTO_PORT_DEF);

        while (!shc.getConnected())
            Sleep(100);
        shc.loginToServer();

        while (true)
            Sleep(100);

        dc.end();
    }

}
// sl_socket.cc - Implementation of SocketHandler class 
//      for operating SOCKET instance.
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/04 21:39:34 by ASMlover.
#ifndef _WINDOWS_
    #include <winsock2.h>
#endif 
#pragma comment(lib, "ws2_32.lib")

#include "../inc/sl_socket.h"

namespace sl {

#define SL_BACKLOG_DEF      (SOMAXCONN) // default listen backlog 

    bool WSLib::isLoaded_ = false;
    bool WSLib::load(void)
    {
        WSADATA wd = {0};

        if (isLoaded_)
            return true;
        if (0 == WSAStartup(MAKEWORD(2, 2), &wd))
        {
            isLoaded_ = true;
            return true;
        }
        return false;
    }

    void WSLib::unload(void)
    {
        if (isLoaded_)
        {
            WSACleanup();
            isLoaded_ = false;
        }
    }



    SocketHandler::SocketHandler(void)
        : s_(INVALID_SOCKET), h_(NULL)
    {
    }

    SocketHandler::SocketHandler(socket_type s)
        : h_(NULL)
    {
        s_ = s;
    }

    SocketHandler::~SocketHandler(void)
    {
    }

    bool SocketHandler::create(void)
    {
        close();

        s_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        return (INVALID_SOCKET != s_);
    }

    bool SocketHandler::conenctTo(const char* address, ushort port)
    {
        struct sockaddr_in sa = {0};

        if (INVALID_SOCKET != s_ && NULL != address)
        {
            sa.sin_addr.s_addr = inet_addr(address);
            sa.sin_family      = AF_INET;
            sa.sin_port        = htons(port);

            if (SOCKET_ERROR == connect(s_, (struct sockaddr*)&sa, sizeof(sa)))
            {
                int err = WSAGetLastError();
                if (WSAEWOULDBLOCK != err)
                    return false;
            }
            return true;
        }
        return false;
    }

    bool SocketHandler::bindTo(const char* address, ushort port)
    {
        struct sockaddr_in sa = {0};

        sa.sin_addr.s_addr = (NULL != address ? inet_addr(address) : htonl(INADDR_ANY));
        sa.sin_family      = AF_INET;
        sa.sin_port        = htons(port);
        
        return (INVALID_SOCKET != s_ 
            && SOCKET_ERROR != bind(s_, (struct sockaddr*)&sa, sizeof(sa)) 
            && SOCKET_ERROR != listen(s_, SL_BACKLOG_DEF));
    }

    bool SocketHandler::acceptFrom(struct sockaddr* from, SocketHandler* sh)
    {
        struct sockaddr_in sa = {0};
        int    fromlen = sizeof(sa);
        
        if (INVALID_SOCKET != s_ && NULL != sh)
        {
            socket_type s = accept(s_, 
                (NULL != from ? from : (struct sockaddr*)&sa), &fromlen);
            sh->s_ = s;
            sh->h_ = h_;

            return (INVALID_SOCKET != s);
        }
        return false;
    }

    void SocketHandler::close(void)
    {
        if (INVALID_SOCKET != s_)
        {
            shutdown(s_, SD_BOTH);
            closesocket(s_);
            s_ = INVALID_SOCKET;
        }
    }

    bool SocketHandler::read(int outlen, char* outbuf)
    {
        int readlen = 0, readret = 0;

        if (INVALID_SOCKET != s_ && NULL != outbuf && outlen > 0)
        {
            do 
            {
                readret = recv(s_, outbuf + readlen, outlen - readlen, 0);
                if (SOCKET_ERROR == readret)
                {
                    int err = WSAGetLastError();
                    if (WSAECONNRESET == err || WSAENOTSOCK == err)
                        return false;
                }
                else if (0 == readret)
                    return false;
                else
                    readlen += readret;
            } while (readlen != outlen);
            return true;
        }
        return false;
    }

    bool SocketHandler::write(const char* inbuf, int inlen)
    {
        int writelen = 0, writeret = 0;

        if (INVALID_SOCKET != s_ && NULL != inbuf && inlen > 0)
        {
            do 
            {
                writeret = send(s_, inbuf + writelen, inlen - writelen, 0);
                if (SOCKET_ERROR == writeret)
                {
                    if (WSAEWOULDBLOCK != WSAGetLastError())
                        return false;
                } 
                else 
                    writelen += writeret;
            } while (writelen != inlen);
            return true;
        }
        return false;
    }

    void SocketHandler::setWindowHandle(const void* h)
    {
        h_ = (void*)h;
    }

    bool SocketHandler::selEvent(long ev, uint msg)
    {
        return ((NULL != h_ && INVALID_SOCKET != s_) ? 
            SOCKET_ERROR != WSAAsyncSelect(s_, (HWND)h_, msg, ev) : false);
    }

    const socket_type SocketHandler::getSHandler(void) const 
    {
        return s_;
    }

    void SocketHandler::eventConnect(SocketHandler* sh)
    {
    }
    void SocketHandler::eventAccept(SocketHandler* sh)
    {
    }
    void SocketHandler::eventRead(SocketHandler* sh)
    {
    }
    void SocketHandler::eventWrite(SocketHandler* sh)
    {
    }
    void SocketHandler::eventClose(SocketHandler* sh)
    {
    }
    
}
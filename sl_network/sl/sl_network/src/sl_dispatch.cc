// sl_dispatch.cc - Implementation of dispatch class for 
//      dispatching client/server socket handler.
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/04 22:59:24 by ASMlover.
#include <windows.h>
#include <tchar.h>
#if _MT
    #include <process.h>
#endif
#include <rpc.h>
#pragma comment(lib, "rpcrt4.lib")
#include "../inc/sl_socket.h"

#include "../inc/sl_dispatch.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
    #pragma warning(disable:4996)
#endif


namespace sl {

#if _MT
    #define THREAD_NEW(_call, _args) (handle_type)_beginthreadex(NULL, 0, _call, _args, 0, NULL)
#else
    #define THREAD_NEW(_call, _args) (handle_type)CreateThread(NULL, 0, _call, _args, 0, NULL)
#endif


    Dispatch::Dispatch(void)
        : sh_(NULL), h_(NULL), hThread_(NULL), hEvent_(NULL)
    {
        memset(sName_, 0, sizeof(sName_));
        WSLib::load();
    }

    Dispatch::~Dispatch(void)
    {
        WSLib::unload();
    }

    bool Dispatch::setSHandler(SocketHandler* sh)
    {
        if (NULL != sh)
        {
            sh_ = sh;
            return true;
        }
        return false;
    }

    SocketHandler* Dispatch::getSHandler(void) const
    {
        return sh_;
    }

    bool Dispatch::begin(const char* address, ushort port)
    {
        return getUuidStr(50, sName_);
    }

    void Dispatch::end(void)
    {
        if (NULL != h_ && NULL != hThread_)
        {
            if (NULL != sh_)
                sh_->selEvent(0L, 0);
            SendMessage((HWND)h_, WM_CLOSE, 0, 0);
            WaitForSingleObject(hThread_, INFINITE);
            UnregisterClass(sName_, NULL);
            SAFEHANDLE_FREE(hThread_);
        }
        SAFEHANDLE_FREE(hEvent_);

        if (NULL != sh_)
            sh_->close();
        h_ = NULL;
    }

    return_type CALLBACK Dispatch::sWndThread(void* args)
    {
        Dispatch* self = (Dispatch*)args;
        MSG       msg  = {0};

        if (NULL == self)
            return -1;
        self->h_ = self->windowNew();
        self->sh_->setWindowHandle(self->h_);
        SetEvent(self->hEvent_);

        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return 0;
    }

    bool Dispatch::createMsgLoop(void)
    {
        hEvent_  = CreateEvent(NULL, TRUE, FALSE, NULL);
        hThread_ = THREAD_NEW(&Dispatch::sWndThread, this);
        
        return (NULL != hEvent_ && NULL != hThread_);
    }

    bool Dispatch::waitForMsgLoopRun(ulong millitm)
    {
        return (WAIT_OBJECT_0 == WaitForSingleObject(hEvent_, millitm));
    }

    hwnd_type Dispatch::windowNew(void)
    {
        WNDCLASSEX wc = {0};
        HWND       h  = NULL;

        wc.cbSize        = sizeof(WNDCLASSEX);
        wc.lpfnWndProc   = (WNDPROC)&Dispatch::sWndProc;
        wc.lpszClassName = sName_;
        if (!RegisterClassEx(&wc))
            return NULL;

        h = CreateWindowEx(0, sName_, NULL, WS_OVERLAPPEDWINDOW, 
            0, 0, 0, 0, NULL, NULL, NULL, this);
        ShowWindow(h, SW_HIDE);
        UpdateWindow(h);

        return (hwnd_type)h;
    }

    bool Dispatch::getUuidStr(size_t strLen, sl_char* outStr)
    {
        sl_char* s = NULL;
        UUID uid = {0};
        bool ret = false;

        if (strLen > 0 && NULL != outStr && RPC_S_OK == UuidCreate(&uid))
        {
            if (RPC_S_OK == UuidToString(&uid, (sl_str*)&s))
            {
                size_t len = _tcslen(s);
                if (len <= strLen)
                {
                    _tcsncpy(outStr, s, len);
                    ret = true;
                }
                RpcStringFree((sl_str*)&s);
            }
        }
        return ret;
    }

    long CALLBACK Dispatch::sWndProc(hwnd_type h, uint msg, wparam_type wp, lparam_type lp)
    {
        if (WM_CREATE == msg)
        {
            Dispatch* self = (Dispatch*)((CREATESTRUCT*)lp)->lpCreateParams;
            SetWindowLong((HWND)h, GWL_USERDATA, (LONG)self);
        }
        else 
        {
            Dispatch* self = (Dispatch*)GetWindowLong((HWND)h, GWL_USERDATA);
            if (NULL != self)
                self->wndProc(msg, wp, lp);
        }

        return DefWindowProc((HWND)h, msg, wp, lp);
    }

    long Dispatch::wndProc(uint msg, wparam_type wp, lparam_type lp)
    {
        switch (msg)
        {
        case WM_CLOSE:
            DestroyWindow((HWND)h_);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case UM_DISPATCH:
            dispEventProc(wp, lp);
            break;
        }

        return 0L;
    }

    long Dispatch::dispEventProc(wparam_type wp, lparam_type lp)
    {
        WORD err = WSAGETSELECTERROR(lp);
        SocketHandler s((socket_type)wp);

        s.setWindowHandle(h_);
        if (NULL != sh_)
        {
            if (err)
            {
                WSAECONNABORTED == err ? sh_->eventClose(&s) : s.close();
                return -1L;
            }

            switch (WSAGETSELECTEVENT(lp))
            {
            case FD_CONNECT:
                sh_->eventConnect(&s);
                break;
            case FD_ACCEPT:
                sh_->eventAccept(&s);
                break;
            case FD_READ:
                sh_->eventRead(&s);
                break;
            case FD_WRITE:
                sh_->eventWrite(&s);
                break;
            case FD_CLOSE:
                sh_->eventClose(&s);
                break;
            }
            return 0L;
        }
        return -1L;
    }



    bool Dispatch_C::begin(const char* address, ushort port)
    {
        if (Dispatch::begin() && createMsgLoop())
        {
            SocketHandler* sh = getSHandler();
            waitForMsgLoopRun(INFINITE);
            if (NULL != sh)
            {
                sh->create();
                sh->selEvent(FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
                sh->conenctTo(address, port);
               
                return true;
            }
        }
        return false;
    }

    void Dispatch_C::end(void)
    {
        Dispatch::end();   
    }


    bool Dispatch_S::begin(const char* address, ushort port)
    {
        if (Dispatch::begin() && createMsgLoop())
        {
            SocketHandler* sh = getSHandler();
            waitForMsgLoopRun(INFINITE);
            if (NULL != sh)
            {
                sh->create();
                sh->bindTo(address, port);
                sh->selEvent(FD_ACCEPT | FD_READ | FD_WRITE | FD_CLOSE);
                
                return true;
            }
        }
        return false;
    }

    void Dispatch_S::end(void)
    {
        Dispatch::end();
    }

}
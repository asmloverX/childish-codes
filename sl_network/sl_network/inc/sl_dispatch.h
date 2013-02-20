// sl_dispatch.h - Dispatch class for dispatching client/server
//      socket handler.
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/04 22:30:17 by ASMlover.
#ifndef __SL_DISPATCH_HEADER_H__
#define __SL_DISPATCH_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#ifndef _SL_DISPATCH_
#define _SL_DISPATCH_

namespace sl {

#if _WIN32 || _WIN64
    #ifndef SAFEHANDLE_FREE
        #define SAFEHANDLE_FREE(_h) { if (NULL != (_h)) { CloseHandle((_h)); (_h) = NULL; } }
    #endif

    #ifndef CALLBACK
        #define CALLBACK __stdcall
    #endif

    #ifdef _MT
        typedef unsigned int return_type;
    #else
        typedef unsigned long return_type;
    #endif
#endif

    typedef unsigned short ushort;
    typedef unsigned int   uint;
    typedef unsigned long  ulong;
    typedef void*          handle_type;
#if _WIN64
    typedef unsigned __int64 wparam_type;
    typedef __int64          lparam_type;
#else
    typedef __w64 unsigned int wparam_type;
    typedef __w64 long         lparam_type;
#endif
#ifndef WIN_INTERNAL
    typedef struct hwnd_type__ { int unused; }* hwnd_type;
#endif
#ifdef UNICODE
    typedef ushort* sl_str;
    typedef wchar_t sl_char;
#else
    typedef unsigned char* sl_str;
    typedef char sl_char;
#endif

    class SocketHandler;
    class Dispatch
    {
        SocketHandler* sh_;     // socket handler
        hwnd_type   h_;         // handle of window
        handle_type hThread_;   // handle of thread for sThread
        handle_type hEvent_;    // handle of event for sThread
        sl_char     sName_[50]; // name of window class
    public:
        Dispatch(void);
        virtual ~Dispatch(void);
    public:
        bool setSHandler(SocketHandler* sh);
        SocketHandler* getSHandler(void) const;
        
        virtual bool begin(const char* address = 0, ushort port = 0);
        virtual void end(void);   
    protected: 
        // Thread for inner window, in the inner window, 
        // we should call createMsgLoop() to create the 
        // message loop.
        static return_type CALLBACK sWndThread(void* args);

        bool createMsgLoop(void);

        // Wait for message loop running, then we can 
        // do other things in another threads.
        bool waitForMsgLoopRun(ulong millitm);
    private:
        // Create a new inner window instance for 
        // the dispatch instance. 
        // A handle to the new window indicates success, 
        // NULL indicates failure.
        hwnd_type windowNew(void);

        // Call getUuidStr member function to get uuid string 
        // for window class name. If no error occurs, this function 
        // returns true, or the return value is false.
        bool getUuidStr(size_t srtLen, sl_char* outStr);
        static long CALLBACK sWndProc(hwnd_type h, uint msg, wparam_type wp, lparam_type lp);
        long wndProc(uint msg, wparam_type wp, lparam_type lp);

        long dispEventProc(wparam_type wp, lparam_type lp);
    };


    // Dispatch class for client socket handler
    // 
    // example:
    //      Suppose class ClientSHandler extends from class SocketHandler.
    //      class ClientSHandler 
    //          : public sl::SocketHandler
    //      {
    //          ...
    //      };
    //
    //      ClientSHandler csh;
    //      sl::Dispatch_c dc;
    //  1> set client socket for Dispatch_C instance
    //      dc.setSHandler(&csh);
    //  2> then we can dispath the ClientSHandler instance
    //      dc.begin(serverNetIpAddress, serverNetPort);
    //  3> end dispatch of instance of class ClientSHandler after use
    //      dc.end();
    class Dispatch_C 
        : public Dispatch 
    {
    public:
        bool begin(const char* address = "127.0.0.1", ushort port = 5555);
        void end(void);
    };

    // Dispatch class for serevr socket handler
    // 
    // example:
    //      Suppose class ServerSHandler extends from class SocketHandler.
    //      class ServerSHandler 
    //          : public sl::SocketHandler
    //      {
    //          ... 
    //      };
    //  
    //      ServerSHandler ssh;
    //      sl::Dispatch_S ds;
    //  1> set server socket handler for Dispatch_S instance
    //      ds.setSHandler(&ssh);
    //  2> dispatching instance of class ServerSHandler
    //      ds.begin(NULL, serverNetPort);
    //     or
    //      ds.begin(toBindServerNetIpAddress, serverNetPort);
    //  3> end of dispatching server socket handler after use
    //      ds.end();
    class Dispatch_S 
        : public Dispatch 
    {
    public:
        bool begin(const char* address = NULL, ushort port = 5555);
        void end(void);
    };

}

#endif  // _SL_DISPATCH_

#endif  // __SL_DISPATCH_HEADER_H__
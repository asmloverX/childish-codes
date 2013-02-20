// sl_socket.h - SocketHandler class for operating SOCKET.
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/04 21:37:38 by ASMlover.
#ifndef __SL_SOCKET_HEADER_H__
#define __SL_SOCKET_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#ifndef _SL_SOCKET_
#define _SL_SOCKET_


struct sockaddr;
namespace sl {

#ifndef UM_DISPATCH
    #define UM_DISPATCH (0x0400 + 1234)     // #define WM_USER  (0x0400)
#endif

    typedef unsigned short ushort;
    typedef unsigned int   uint;
    typedef uint socket_type;

    // load/unload windows network library
    class WSLib
    {
        static bool isLoaded_;
    public:
        // Call this function to load windows sockets.
        static bool load(void);
        // Call this function to unload windows socktes.
        static void unload(void);
    };

    class SocketHandler 
    {
        socket_type s_; // SOCKET instance
        void*       h_; // handle to inner window
    public:
        SocketHandler(void);
        SocketHandler(socket_type s);
        virtual ~SocketHandler(void);
    public:
        // Call the create member function after constructing a 
        // SocketHandler object to create SOCKET. If no error 
        // occurs, this function returns true, or the return value
        // is false.
        bool create(void);

        // Connect to a remote service. The remote service's IP
        // address is address and it's network port is port.
        // If no error ocuurs, this function returns true, or the 
        // return value is false.
        bool conenctTo(const char* address, ushort port);

        // Call this member function to assciate a local address 
        // with the socket handler.If no error occurs, this function 
        // returns true, or the return values is false.
        bool bindTo(const char* address, ushort port);

        // Accept new connections from other side of network. 
        // sh identifying a new socket handler that is avavilable
        // for connection. If no error occurs returns true, or 
        // the return value is false.
        bool acceptFrom(struct sockaddr* from, SocketHandler* sh);

        // Close the socket handler.
        void close(void);

        // Read out outlen bytes message from this socket handler.
        // If no error occurs, this function returns true, or 
        // the return value is false.
        bool read(int outlen, char* outbuf);

        // Write inlen bytes message to other side of network by 
        // this socket handler. If no error occurs, this function
        // returns true, or the return value is false.
        bool write(const char* inbuf, int inlen);

        void setWindowHandle(const void* h);

        // Call selEvent member function to request network event 
        // notification for a socket handler.
        bool selEvent(long ev, uint msg = UM_DISPATCH);

        const socket_type getSHandler(void) const;
    public:
        // Called by the framework to notify this connecting socket 
        // handler that its connection attempt is completed, whether 
        // successfully or in error.
        virtual void eventConnect(SocketHandler* sh);

        // Called by the framework to notify a listening socket handler 
        // that it can accept pending connection requests by calling 
        // the acceptFrom member function.
        virtual void eventAccept(SocketHandler* sh);

        // Called by the framework to notify this socket handler 
        // that there is data in the buffer that can by received 
        // by call the read member function.
        virtual void eventRead(SocketHandler* sh);

        // Called by the framework to notify the socket handler 
        // that it can now send data by calling write member function.
        virtual void eventWrite(SocketHandler* sh);

        // Called by the framework to notify this socket handler 
        // that the connected socket handler is closed by its process.
        virtual void eventClose(SocketHandler* sh);
    };
}

#endif  // _SL_SOCKET_

#endif  // __SL_SOCKET_HEADER_H__
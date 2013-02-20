// sl_network_test_client.cc - testing class sl::SocketHandler 
//      and sl::Dispatch_C
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/05 13:32:43 by ASMlover.
#include <windows.h>
#include <vector>
#include "../../inc/sl_socket.h"
#include "../../inc/sl_dispatch.h"

#include "sl_network_test.h"


namespace sl_test {

    class CltHandler 
        : public sl::SocketHandler 
    {
    public:
        void eventConnect(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_CONNECT\n");

            char* str = "Now I Can Send Message To You!";
            sendMsg(MSG_TYPE_STR, str, (int)strlen(str) + 1);
        }

        void eventRead(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_READ\n");
        }

        void eventWrite(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_WRITE\n");
        }

        void eventClose(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_CLOSE\n");
            close();
        }
    private:
        bool sendMsg(word_t type, const char* dat, int len)
        {
            char buf[100] = {0};
            struct __msg_head h = {0};

            h.len_  = len;
            h.type_ = type;

            memcpy(buf, &h, sizeof(h));
            memcpy(buf + sizeof(h), dat, len);

            return write(buf, len + sizeof(h));
        }
    };

    void 
    sl_network_client(void)
    {
        CltHandler ch;
        sl::Dispatch_C dc;

        dc.setSHandler(&ch);
        dc.begin();

        while (true)
            Sleep(100);
        dc.end();
    }

}
// sl_network_test_server.cc - testing class sl::SocketHandler 
//      and sl::Dispatch_S
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/05 11:19:19 by ASMlover.
#include <windows.h>
#include <vector>
#include "../../inc/sl_socket.h"
#include "../../inc/sl_dispatch.h"

#include "sl_network_test.h"



namespace sl_test {

    class SrvHandler 
        : public sl::SocketHandler
    {
        std::vector<sl::SocketHandler> shList_;

        struct __msg_head h_;
        bool              hasHead_;
    public:
        SrvHandler(void)
            : hasHead_(false)
        {
            shList_.clear();
            memset(&h_, 0, sizeof(h_));
        }

        ~SrvHandler(void)
        {
        }
    public:
        void eventAccept(sl::SocketHandler* s)
        {
            SocketHandler sh;

            fprintf(stdout, "response FD_ACCEPT ... \n");

            if (acceptFrom(NULL, &sh)) 
            {
                sh.selEvent(FD_READ | FD_WRITE | FD_CLOSE);
                shList_.push_back(sh);
            }
        }

        void eventRead(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_READ ... \n");
            if (!hasHead_)
            {
                s->read(sizeof(h_), (char*)&h_);
                hasHead_ = true;

                fprintf(stdout, "read head.len_ = %d, head.type_ = %x\n", 
                    h_.len_, h_.type_);
            }
            else
            {
                char buf[100] = {0};
                s->read(h_.len_, buf);
                if (MSG_TYPE_STR == h_.type_)
                    fprintf(stdout, "read message is [%s]\n", buf);
                memset(&h_, 0, sizeof(h_));
                hasHead_ = false;
            }
        }

        void eventWrite(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_WRITE ... \n");
        }

        void eventClose(sl::SocketHandler* s)
        {
            fprintf(stdout, "response FD_CLOSE ... SHandler = %d\n", s->getSHandler());

            if (s->getSHandler() == getSHandler()) 
            {
                closeAllClients();
                close();
            }
            else 
            {
                std::vector<sl::SocketHandler>::iterator i;
                for (i = shList_.begin(); i != shList_.end(); )
                {
                    if (i->getSHandler() == s->getSHandler())
                    {
                        s->close();
                        i = shList_.erase(i);
                    }
                    else
                        ++i;
                }
            }
        }

    private:
        void closeAllClients(void)
        {
            for (int i = 0; i < (int)shList_.size(); ++i)
                shList_[i].close();
            shList_.clear();
        }
    };

    void 
    sl_network_server(void)
    {
        sl::Dispatch_S ds;
        SrvHandler     sh;

        ds.setSHandler(&sh);
        ds.begin();

        while (true)
            Sleep(100);
        ds.end();
    }

}
// sl_network_test.cc - sl_network testing
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/13 17:22:15 by ASMlover.
#include <stdio.h>
#include "sl_network_test.h"


namespace sl_test {

    static void 
    __sl_network_test_help(void)
    {
        fprintf(stdout, 
        "*************************************************\n"
        "   [0] - quit\n"
        "   [1] - help\n"
        "   [2] - sl_network server test\n"
        "   [3] - sl_network client test\n"
        "   [4] - sl_network packet test\n"
        "   [5] - sl_network packetmgr test\n"
        "   [6] - sl_network capp test\n"
        "   [7] - sl_network sapp test\n"
        "   [8] - sl_network lock test\n"
        "   [9] - sl_network buffer test\n"
        "   [10] - sl_network frame test\n"
        "   [11] - sl_network circularbuffer test\n"
        "   [12] - sl_network version test\n"
        "*************************************************\n"
        );
    }

    void 
    sl_network_test(void)
    {
        int sel = 0;

        fprintf(stdout, "welcome to sl_network, please type \'0\' to quit or \'1\' for help\n");
        while (true)
        {
            fprintf(stdout, "> ");
            fscanf(stdin, "%d", &sel);

            if (0 == sel) {
                fprintf(stdout, "thanks for using sl_network\n");
                break;
            }
            switch (sel)
            {
            case 1:
                __sl_network_test_help();
                break;
            case 2:
                sl_test::sl_network_server();
                break;
            case 3:
                sl_test::sl_network_client();
                break;
            case 4:
                sl_test::sl_network_packet();
                break;
            case 5:
                sl_test::sl_network_packetmgr();
                break;
            case 6:
                sl_test::sl_network_capp();
                break;
            case 7:
                sl_test::sl_network_sapp();
                break;
            case 8:
                sl_test::sl_network_lock();
                break;
            case 9:
                sl_test::sl_network_buffer();
                break;
            case 10:
                sl_test::sl_network_frame();
                break;
            case 11:
                sl_test::sl_network_circularbuffer();
                break;
            case 12:
                sl_test::sl_network_version();
                break;
            default:
                fprintf(stdout, "you typed wrong selection, please type \'0\' to quit or \'1\' for help\n");
            }
            fprintf(stdout, "\n");
        }
    }

}
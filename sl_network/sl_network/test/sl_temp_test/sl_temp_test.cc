// sl_temp_test.cc - Implementation of temp classes and interfaces
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// create at 2011/01/10 09:49:45 by ASMlover.
#include <stdio.h>
#include "sl_temp_test.h"


namespace sl_temp {

    static void 
    __sl_temp_test_help(void)
    {
        fprintf(
            stdout, 
            "********************************************\n"
            "   [0] - quit\n"
            "   [1] - help\n"
            "   [2] - sl_temp hash_map test\n"
            "   [3] - sl_temp creation time of file test\n"
            "********************************************\n"
            );
    }

    void 
    sl_temp_test(void)
    {
        int sel = 0;

        fprintf(stdout, "welcome to sl_temp, please type \'0\' to quit or \'1\' for help\n");
        while (true) {
            fprintf(stdout, "> ");
            fscanf(stdin, "%d", &sel);

            if (0 == sel) {
                fprintf(stdout, "thanks for using sl_temp\n");
                break;
            }
            switch (sel) {
            case 1:
                __sl_temp_test_help();
                break;
            case 2:
                sl_temp_hash_map();
                break;
            case 3:
                sl_temp_creation_time();
                break;
            default:
                fprintf(stdout, "you typed wrong selection, please type \'0\' to quit or \'1\' for help\n");
            }
            printf("\n");
        }
    }
}
// sl_temp_test_creation_time.cc - Testing creation file time
//      use crt function to get file creation time
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/10 10:02:18 by ASMlover.
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "sl_temp_test.h"



namespace sl_temp {

    void 
    sl_temp_creation_time(void)
    {
        struct stat st = {0};
        struct tm*  local = NULL;
        char*  fname = "./sl_temp_test/sl_temp_test_creation_time.cc";

        stat(fname, &st);
        local = localtime(&st.st_ctime);

        fprintf(stdout, 
            "FILE [%s] creation time is : "
            "%.2d/%.2d/%.2d %.2d:%.2d:%.2d\n", 
            fname, local->tm_year + 1900, local->tm_mon + 1, 
            local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
    }

}
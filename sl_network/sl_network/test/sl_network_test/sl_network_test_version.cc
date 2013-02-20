// sl_network_test_version.cc - version test
//
// Copyright (c) 2011,
//      ASMlover. All rights reserved.
//
// created at 2011/01/25 10:17:55 by ASMlover.
#include <stdio.h>
#include "../../inc/sl_version.h"

#include "sl_network_test.h"


namespace sl_test {

    void 
    sl_network_version(void)
    {
        fprintf(stdout, "current library version is: %s\n", 
            sl::sl_version());
    }

}
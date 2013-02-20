// sl_version.cc - Implement of version insterface of 
//      sl_network library.
//
// Copyright (c) 2011,
//      ASMlover. All rights reserved.
//
// created at 2011/01/25 10:12:22 by ASMlover.
#include "../inc/sl_version.h"


namespace sl {

#define SL_NETWORK_VERSION  ("version 0.2")
    
    const char* 
    sl_version(void)
    {
        return SL_NETWORK_VERSION;
    }

}
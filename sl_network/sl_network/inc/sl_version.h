// sl_version.h - Version interface of sl_network library.
//
// Copyright (c) 2011,
//      ASMlover. All rights reserved.
//
// created at 2011/01/25 10:09:30 by ASMlover.
#ifndef __SL_VERSION_HEADER_H__
#define __SL_VERSION_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#ifndef _SL_VERSION_
#define _SL_VERSION_

namespace sl {

    // Call sl_version global function to 
    // get version of sl_network library.
    const char* sl_version(void);

}

#endif  // _SL_VERSION_

#endif  // __SL_VERSION_HEADER_H__
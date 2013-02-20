// sl_temp_test.h - testing temp classes and interfaces 
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/10 09:21:38 by ASMlover.
#ifndef __SL_TEMP_TEST_HEADER_H__
#define __SL_TEMP_TEST_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif 

#ifndef _SL_TEMP_TEST_
#define _SL_TEMP_TEST_

namespace sl_temp {

#if _WIN32 || _WIN64
    #pragma warning(disable:4996)
#endif

    void sl_temp_test(void);

    void sl_temp_hash_map(void);
    void sl_temp_creation_time(void);

}

#endif  // _SL_TEMP_TEST_

#endif  // __SL_TEMP_TEST_HEADER_H__
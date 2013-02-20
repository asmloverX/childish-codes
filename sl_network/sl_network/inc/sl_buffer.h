// sl_buffer.h - Buffer class for reading/writing
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/11 15:17:06 by ASMlover.
#ifndef __SL_BUFFER_HEADER_H__
#define __SL_BUFFER_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#ifndef _SL_BUFFER_
#define _SL_BUFFER_

namespace sl {

    typedef unsigned char byte;
    typedef unsigned int  size_t;

    // Class Buffer
    // 
    // example: 
    //      sl::Buffer b;
    //  1> create a buffer instance
    //      b.create();
    //  2> use write/read functions for reading/writing
    //      short dat[100] = {0};
    //        ... get data 
    //      b.write((byte*)dat, sizeof(dat));
    //        ...
    //      byte buf[100] = {0};
    //      b.read(sizeof(buf), (byte*)buf);
    //  3> release this buffer instance
    //      use "b.release();" after use.
    class Buffer 
    {
        byte*  buf_;        // buffer data
        size_t readPos_;    // reading position
        size_t writePos_;   // writing position
        size_t bufLen_;     // length of buffer
    public:
        Buffer(void);
        ~Buffer(void);
    public:
        // Create buffer class instance with bufLen, 
        // the default bufLen is 1024 bytes, if no error 
        // occurs, this function returns true, or the 
        // return value is false.
        bool create(size_t bufLen = 1024);

        // Release buffer and recover memory of buffer.
        void release(void);

        // Call clear member function to clear buffer space.
        void clear(void);

        // Write inLen bytes data from inBuf to buffer, if no 
        // error occurs, this function returns the number of bytes 
        // written. Or the return value is zero.
        size_t write(const byte* inBuf, size_t inLen);

        // Read outLen bytes data from buffer to outBuf, if no 
        // error occurs, this function returns the number of bytes
        // read. Or the return value is zero.
        size_t read(size_t outLen, byte* outBuf);

        // Remove len bytes data from buffer, if no error occurs, 
        // this function returns the number of bytes removed. Or 
        // the return value is zero.
        size_t remove(size_t len);

        size_t size(void) const;
        size_t dataLength(void) const;
        size_t freeLength(void) const;
    };

}

#endif  // _SL_BUFFER_

#endif  // __SL_BUFFER_HEADER_H__
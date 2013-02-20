// sl_circularbuffer.h - CircularBuffer class for reading/writing
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/13 17:35:37 by ASMlover.
#ifndef __SL_CIRCULAR_BUFFER_HEADER_H__
#define __SL_CIRCULAR_BUFFER_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#ifndef _SL_CIRCULARBUFFER_
#define _SL_CIRCULARBUFFER_

namespace sl {

    typedef unsigned char byte;
    typedef unsigned int  size_t;

    class Buffer;
    class Lock;
    class FrameBlock;

    // class CircularBuffer
    // 
    // example:
    //      sl::CircularBuffer cb;
    //  1> create s circular buffer instance
    //      cb.create();
    //  2> use push/pop member functions for reading/writing data
    //      short dat[100] = {0};
    //          ... set data for dat
    //      cb.push((byte*)dat, sizeof(buf));
    //          ...
    //      sl::FrameBlock fb;
    //      cb.pop(&fb);
    //          ... use these frame block data
    //      fb.release();
    //  3> release this circular buffer instance use 
    //      release member function after use.
    //      cb.release();
    class CircularBuffer 
    {
        Lock*   l_; // multithreading locker for circular buffer
        Buffer* b_; // buffer object for circular buffer
    public:
        CircularBuffer(void);
        ~CircularBuffer(void);
    public:
        // Call the create member funtion after constructing a 
        // circular buffer object to create a bufLen bytes buffer
        // memory. If no error occurs, this function returns true, 
        // or the return value is false.
        bool create(size_t bufLen = 1024);

        // Call the release member function to recover memory 
        // of circular buffer.
        void release(void);

        // Call the clear member function to clear circular buffer.
        void clear(void);

        // Push inLen bytes full frame data from inBuf to circular 
        // buffer. If no error occurs, this function returns true, 
        // or the return value is false.
        bool push(const byte* inBuf, size_t inLen);

        // Push a full frame block data to circular buffer.
        bool push(const FrameBlock* inBlock);

        // Pop a full frame block data from circular buffer to 
        // outBlock. If no error occurs, this function returns 
        // true, or the return value is false.
        bool pop(FrameBlock* outBlock);
    };

}

#endif  // _SL_CIRCULARBUFFER_

#endif  // __SL_CIRCULAR_BUFFER_HEADER_H__
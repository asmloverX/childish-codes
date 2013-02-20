// sl_frame.h - FrameBlock class for full packet data
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/13 16:38:37 by ASMlover.
#ifndef __SL_FRAME_HEADER_H__
#define __SL_FRAME_HEADER_H__

#ifndef __cplusplus
    #error requires C++ compilation (use a .cpp or .cc suffix)
#endif

#ifndef _SL_FRAME_
#define _SL_FRAME_

namespace sl {

    typedef unsigned char byte;
    typedef unsigned int  size_t;

    class FrameBlock 
    {
        size_t size_;   // size of frame block data
        byte*  data_;   // frame block data
    public:
        FrameBlock(void);
        ~FrameBlock(void);
    public:
        // Call allocBlock member function after constructing 
        // a FrameBlock object to allocate memnory space for 
        // frame block data. If no error occurs, this function 
        // returns true, or the return value is false.
        bool allocBlock(size_t blockLen);

        // Call this allocBlock member function after constructing 
        // a FrameBlock object to allocate memory space for frame
        // block data, and write inLen bytes full frame block data 
        // from inBuf ro this FrameBlock object. If no error occurs, 
        // this function returns true, or the return value is false.
        bool allocBlock(const byte* inBuf, size_t inLen);

        // Call freeBlock member function before the destructor to
        // recover memory space of FrameBlock object. In general, 
        // you do not need to call this function, the destructor will
        // do it for you.
        void freeBlock(void);

        // Call isValid member function to get is valid of this frame block.
        bool isValid(void) const;

        // Call get member funtion to get outLen bytes frame block
        // data from FrameBlock object to outBuf. If no error occurs, 
        // this function returns true, or the return value is false.
        bool get(size_t outLen, byte* outBuf);

        // Call this get member function to get data from this 
        // FrameBlock object. Returns NULL if error occurs.
        const byte* get(void) const;

        // Call size member function to get size of this full 
        // frame block data.
        const size_t size(void) const;
    };

}

#endif  // _SL_FRAME_

#endif  // __SL_FRAME_HEADER_H__
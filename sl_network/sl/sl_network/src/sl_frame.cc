// sl_frame.cc - Implementation of FrameBlock class for 
//      full packet data
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/13 16:38:51 by ASMlover.
#include <stdlib.h>
#include <string.h>

#include "../inc/sl_frame.h"


namespace sl {

    FrameBlock::FrameBlock(void)
        : size_(0), data_(NULL)
    {
    }

    FrameBlock::~FrameBlock(void)
    {
        freeBlock();
    }

    bool FrameBlock::allocBlock(size_t blockLen)
    {
        if (blockLen > 0) 
        {
            if (NULL != data_)
                free(data_);
            data_ = (byte*)calloc(blockLen, sizeof(byte));
            if (NULL != data_) 
            {
                size_ = blockLen;
                return true;
            }
        }
        return false;
    }

    bool FrameBlock::allocBlock(const byte* inBuf, size_t inLen)
    {
        if (NULL != inBuf && inLen > 0)
        {
            if (NULL != data_)
                free(data_);
            data_ = (byte*)calloc(inLen, sizeof(byte));
            if (NULL != data_)
            {
                memcpy(data_, inBuf, inLen);
                size_ = inLen;

                return true;
            }
        }
        return false;
    }

    void FrameBlock::freeBlock(void)
    {
        if (NULL != data_)
        {
            free(data_);
            data_ = NULL;
        }
        size_ = 0;
    }

    bool FrameBlock::isValid(void) const
    {
        return (NULL != data_);
    }

    bool FrameBlock::get(size_t outLen, byte* outBuf)
    {
        if (NULL != data_ && NULL != outBuf)
        {
            memcpy(outBuf, data_, (outLen >= size_ ? size_ : outLen));
            return true;
        }
        return false;
    }

    const byte* FrameBlock::get(void) const
    {
        return data_;
    }

    const size_t FrameBlock::size(void) const 
    {
        return size_;
    }

}
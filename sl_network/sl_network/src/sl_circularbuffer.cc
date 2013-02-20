// sl_circularbuffer.cc - Implementation of CircularBuffer 
//      class for reading/writing data
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved.
// 
// created at 2011/01/13 17:45:30 by ASMlover.
#include <stdio.h>
#include "../inc/sl_buffer.h"
#include "../inc/sl_lock.h"
#include "../inc/sl_frame.h"

#include "../inc/sl_circularbuffer.h"



namespace sl {

#ifndef SAFEOBJ_NEW
    #define SAFEOBJ_NEW(_type)  new _type
#endif
#ifndef SAFEOBJ_FREE
    #define SAFEOBJ_FREE(_p) { if (NULL != (_p)) { delete (_p); (_p) = NULL; } }
#endif

    CircularBuffer::CircularBuffer(void)
        : l_(NULL), b_(NULL)
    {
    }

    CircularBuffer::~CircularBuffer(void)
    {
    }

    bool CircularBuffer::create(size_t bufLen)
    {
        l_ = SAFEOBJ_NEW(Lock);
        if (NULL != l_) 
        {
            b_ = SAFEOBJ_NEW(Buffer);
            return (NULL != b_ ? b_->create(bufLen) : false);
        }
        return false;
    }

    void CircularBuffer::release(void)
    {
        if (NULL != b_)
        {
            b_->release();
            SAFEOBJ_FREE(b_);
        }
        SAFEOBJ_FREE(l_);
    }

    void CircularBuffer::clear(void)
    {
        if (NULL != b_ && NULL != l_)
        {
            l_->lock();
            b_->clear();
            l_->unlock();
        }
    }

    bool CircularBuffer::push(const byte* inBuf, size_t inLen)
    {
        size_t pushLen = 0;
        
        if (NULL != b_ && NULL != l_ && NULL != inBuf 
            && inLen > 0 && inLen <= b_->size())
        {
            l_->lock();
            while (b_->freeLength() < sizeof(size_t) + inLen)
            {
                size_t removeLen = 0;
                b_->read(sizeof(size_t), (byte*)&removeLen);
                b_->remove(removeLen);
            }
            pushLen = b_->write((byte*)&inLen, sizeof(size_t));
            pushLen += b_->write(inBuf, inLen);
            l_->unlock();
        }
        return (pushLen == (sizeof(size_t) + inLen));
    }

    bool CircularBuffer::push(const FrameBlock* inBlock)
    {
        return (NULL != inBlock ? push(inBlock->get(), inBlock->size()) : false);
    }

    bool CircularBuffer::pop(FrameBlock* outBlock)
    {
        size_t popLen = 0, blockLen = 0;
        bool   ret = false;

        if (NULL != b_ && NULL != l_ && NULL != outBlock)
        {
            l_->lock();
            popLen = b_->read(sizeof(size_t), (byte*)&blockLen);
            if (sizeof(size_t) == popLen)
            {
                popLen += (outBlock->allocBlock(blockLen) ? 
                    b_->read(outBlock->size(), (byte*)outBlock->get()) : 0);
                ret = (popLen == (sizeof(size_t) + blockLen));
            }
            l_->unlock();
        }
        return ret;
    }

}
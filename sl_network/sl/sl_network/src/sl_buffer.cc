// sl_buffer.cc - Implementation of buffer class for 
//      reading/writing
// 
// Copyright (c) 2011, 
//      ASMlover. All rights reserved. 
// 
// created at 2011/01/11 15:17:22 by ASMlover.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/sl_buffer.h"


namespace sl {

#ifndef SL_BUFSIZE_DEF
    #define SL_BUFSIZE_DEF  (1024)  // default bytes of buffer
#endif

    Buffer::Buffer(void) 
        : buf_(NULL), readPos_(0), writePos_(0), bufLen_(0)
    {
    }

    Buffer::~Buffer(void)
    {
    }

    bool Buffer::create(size_t bufLen)
    {
        bufLen_ = (bufLen < SL_BUFSIZE_DEF ? SL_BUFSIZE_DEF : bufLen);
        buf_ = (byte*)calloc(bufLen_, sizeof(byte));

        return (NULL != buf_);
    }

    void Buffer::release(void)
    {
        if (NULL != buf_)
        {
            free(buf_);
            buf_ = NULL;
        }
        bufLen_ = readPos_ = writePos_ = 0;
    }

    void Buffer::clear(void)
    {
        if (NULL != buf_)
            memset(buf_, 0, bufLen_);
        readPos_ = writePos_ = 0;
    }

    size_t Buffer::write(const byte* inBuf, size_t inLen)
    {
        if (NULL != buf_ && NULL != inBuf && inLen > 0)
        {
            size_t freeLen  = freeLength();
            size_t writeLen = (freeLen >= inLen ? inLen : freeLen);

            if (readPos_ > writePos_)
            {
                memcpy(buf_ + writePos_, inBuf, writeLen);
                writePos_ += writeLen;
            }
            else 
            {
                size_t leftLen = 0, tailLen = bufLen_ - writePos_;
                tailLen = (tailLen >= writeLen ? writeLen : tailLen);
                leftLen = writeLen - tailLen;

                memcpy(buf_ + writePos_, inBuf, tailLen);
                writePos_ = (writePos_ + tailLen) % bufLen_;
                if (leftLen > 0)
                {
                    memcpy(buf_, inBuf + tailLen, leftLen);
                    writePos_ += leftLen;
                }
            }
            return writeLen;
        }
        return 0;
    }

    size_t Buffer::read(size_t outLen, byte* outBuf)
    {
        if (NULL != buf_ && NULL != outBuf && outLen > 0)
        {
            size_t dataLen = dataLength();
            size_t readLen = (dataLen >= outLen ? outLen : dataLen);

            if (writePos_ > readPos_)
            {
                memcpy(outBuf, buf_ + readPos_, readLen);
                readPos_ += readLen;
            }
            else 
            {
                size_t leftLen = 0, tailLen = bufLen_ - readPos_;
                tailLen = (tailLen >= readLen ? readLen : tailLen);
                leftLen = readLen - tailLen;

                memcpy(outBuf, buf_ + readPos_, tailLen);
                readPos_ = (readPos_ + tailLen) % bufLen_;
                if (leftLen > 0) 
                {
                    memcpy(outBuf + tailLen, buf_, leftLen);
                    readPos_ += leftLen;
                }
            }
            return readLen;
        }
        return 0;
    }

    size_t Buffer::remove(size_t len)
    {
        if (NULL != buf_)
        {
            size_t dataLen = dataLength();
            size_t removeLen = (dataLen >= len ? len : dataLen);

            if (writePos_ > readPos_)
                readPos_ += removeLen;
            else 
            {
                size_t tailLen = bufLen_ - readPos_;
                tailLen = (tailLen >= removeLen ? removeLen : tailLen);

                readPos_ = (readPos_ + tailLen) % bufLen_;
                readPos_ += removeLen - tailLen;
            }
            return removeLen;
        }
        return 0;
    }

    size_t Buffer::size(void) const 
    {
        return bufLen_;
    }

    size_t Buffer::freeLength(void) const 
    {
        return ((readPos_ > writePos_) ? 
            (readPos_ - writePos_) : (bufLen_ - (writePos_ - readPos_)));
    }

    size_t Buffer::dataLength(void) const 
    {
        return ((writePos_ >= readPos_) ? 
            (writePos_ - readPos_) : (bufLen_ - (readPos_ - writePos_)));
    }

}
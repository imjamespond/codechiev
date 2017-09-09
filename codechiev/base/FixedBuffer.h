//
//  FixedBuffer.hpp
//  codechiev
//
//  Created by metasoft on 16/7/20.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef FixedBuffer_hpp
#define FixedBuffer_hpp

#include <stdio.h>
#include <string>
#include <string.h>
#include <assert.h>

namespace codechiev {
    namespace base {

        template <int BUFFER_SIZE>
        class FixedBuffer
        {
        public:
            FixedBuffer():writer_(0),reader_(0),writer_size_(BUFFER_SIZE-1)
            {
                ::memset(buffer_, 0, BUFFER_SIZE);
            }

            int readable()
            {
                int rb = writer_-reader_;
                assert(rb>=0);
                return rb;
            }

            int writable()
            {
                int wb = writer_size_-writer_;
                assert(wb>=0);
                return wb;
            }

            int append(const char *str)
            {
                return append(str, static_cast<int>(::strlen(str)));
            }
            int append(const char *str, int len)
            {
                if(len<=0)
                    return 0;
                int wb = writable();
                if(wb>len)
                {
                    ::memcpy(buffer_+writer_, str, len);
                    write(len);
                    return len;
                }else if(wb>0)
                {
                    ::memcpy(buffer_+writer_, str, wb);
                    write(wb);
                    return wb;
                }
            }

            void read(int len)
            {
                reader_+=len;
            }

            void readall()
            {
                reader_=0;
                writer_=0;
                ::memset(buffer_, 0, BUFFER_SIZE);
            }

            void write(int len)
            {
                writer_+=len;
                assert(writer_<BUFFER_SIZE);
                buffer_[writer_]=0;
            }

            void move()
            {
                int rb = readable();
                ::memcpy(buffer_, buffer_+reader_, rb);
                reader_=0;
                writer_=rb;
                buffer_[writer_]=0;
            }

            char* buffer()
            {
                return buffer_+writer_;
            }

            const char* str()
            {
                return buffer_+reader_;
            }

            inline int writer(){return writer_;}
            inline int reader(){return reader_;}
        private:
            int writer_;
            int reader_;
            const int writer_size_;
            char buffer_[BUFFER_SIZE];
        };

    }
}

#endif /* FixedBuffer_hpp */

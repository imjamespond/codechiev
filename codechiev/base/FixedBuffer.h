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
#include <assert.h>

namespace codechiev {
    namespace base {
        
        template <int BUFFER_SIZE>
        class FixedBuffer
        {
        public:
            FixedBuffer():writer_(0),reader_(0)
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
                int wb = BUFFER_SIZE-writer_;
                assert(wb>=0);
                return wb;
            }
            
            void append(const char *str)
            {
                int len = static_cast<int>(::strlen(str));
                int wb = writable();
                if(wb>len)
                {
                    ::memcpy(buffer_+writer_, str, len);
                    write(len);
                }else if(wb>0)
                {
                    wb-=1;
                    ::memcpy(buffer_+writer_, str, wb);
                    write(wb);
                }
            }
            
            void read(int len)
            {
                int rb = readable();
                if(rb>=len)
                {
                    reader_+=len;
                }else
                {
                    reader_+=rb;
                }
            }
            
            void write(int len)
            {
                writer_+=len;
                assert(writer_<BUFFER_SIZE);
                buffer_[writer_]='\0';
            }
            
            void move()
            {
                int len = writer_ - reader_;
                assert(len>=0);
                ::memcpy(buffer_, buffer_+reader_, len);
                reader_=0;
                writer_=len;
            }
            
            const char* buffer()
            {
                return buffer_+reader_;
            }
            
        private:
            int writer_;
            int reader_;
            char buffer_[BUFFER_SIZE];
        };
        
    }
}

#endif /* FixedBuffer_hpp */

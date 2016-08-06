//
//  TcpLengthCode.h
//  
//
//  Created by metasoft on 16/8/5.
//
//

#ifndef TcpLengthCode_h
#define TcpLengthCode_h

#include <boost/noncopyable.hpp>
#include <stdint.h>
#include <string.h>
#include <base/Logger.hpp>
#include "Channel.hpp"

namespace codechiev {
    namespace net {
     
        template <int HEADER>
        class TcpLengthCoder : public boost::noncopyable
        {
        public:
            static bool decode(Channel* channel, std::string& msg)
            {
                int readable = channel->getReadBuf()->readable();
                
                if(readable>=HEADER)
                {
                    int32_t length,whole;
                    ::memcpy(&length, channel->getReadBuf()->str(), sizeof(int32_t));
                    whole = length + HEADER;
                    
                    if(readable>=whole)
                    {
                        msg.append(channel->getReadBuf()->str()+HEADER, length);
                        channel->getReadBuf()->read(whole);
                        channel->getReadBuf()->move();
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            
            static void encode(Channel *channel, size_t len)
            {
                char header[HEADER];
                ::memcpy(header, &len, HEADER);
                channel->getWriteBuf()->append(header, HEADER);
            }
        };
        
    }
}

#endif /* TcpLengthCode_h */

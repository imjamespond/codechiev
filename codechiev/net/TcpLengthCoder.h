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
                    uint32_t length,whole;
                    ::memcpy(&length, channel->getReadBuf()->str(), sizeof(uint32_t));
                    length = NetworkToHost32(length);
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
            
            static void encode(Channel *channel, size_t length)
            {
                uint32_t header;
                header = HostToNetwork32(static_cast<uint32_t>(length));
                channel->getWriteBuf()->append(static_cast<char*>(&header), sizeof(uint32_t));
            }
        };
        
    }
}

#endif /* TcpLengthCode_h */

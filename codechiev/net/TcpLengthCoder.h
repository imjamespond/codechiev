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
#include "Channel.hpp"

namespace codechiev {
    namespace net {
     
        template <int LENGTH>
        class TcpLengthCoder : public boost::noncopyable
        {
        public:
            static bool decode(Channel* channel, std::string& msg)
            {
                int readable = channel->getReadBuf()->readable();
                
                if(readable>=4)
                {
                    int32_t length,whole;
                    ::memcpy(&length, channel->getReadBuf()->str(), sizeof(int32_t));
                    whole = length + 4;
                    if(readable>=whole)
                    {
                        std::string msg;
                        msg.append(channel->getReadBuf()->str()+4, length);
                        channel->getReadBuf()->read(whole);
                        
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
        }
        
    }
}

#endif /* TcpLengthCode_h */

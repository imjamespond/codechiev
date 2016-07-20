//
//  Logger.hpp
//  codechiev
//
//  Created by metasoft on 16/7/20.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Logger_hpp
#define Logger_hpp

#include "FixedBuffer.h"

namespace codechiev {
    namespace base {
        
        class Logger
        {
        public:
            enum Type
            {
                Trace,
                Debug,
                Info,
                Warn,
                Error,
                TypeSize,
            };
            explicit Logger(Type);
        private:
            Type type_;
        };
        
        class LoggerStream
        {
        public:
            LoggerStream();
            
            
            
        private:
            FixedBuffer<256> buffer;
        };
    }
}

#endif /* Logger_hpp */

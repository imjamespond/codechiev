//
//  Time.hpp
//  codechiev
//
//  Created by metasoft on 16/7/19.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Time_hpp
#define Time_hpp

#include <stdint.h>
#include <string>

namespace codechiev {
    namespace base {
        
        static const int64_t MILLIS_SEC = 1000;
        static const int64_t MICROS_SEC = MILLIS_SEC * 1000;
        static const int64_t NANOS_SEC = MICROS_SEC * 1000;
        static const float SEC_OF_MILLI = .001;
        
        class Time{
        public:
            Time(int64_t t = 0);
            
            static Time Now();
            static Time NowTm();
            static Time GetClockTime();
            
            static void
            SleepMillis(int64_t);
            
            static std::string
            GetSimpleString();
            
            inline int64_t
            getMillis(){return timeMillis_;}
            
            void
            operator=(int64_t);
            
            int64_t
            operator-(Time&);
        private:
            int64_t timeMillis_;
        };
    }
}

#endif /* Time_hpp */

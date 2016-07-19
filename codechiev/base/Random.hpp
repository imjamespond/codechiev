//
//  Random.hpp
//  codechiev
//
//  Created by metasoft on 16/7/19.
//  Copyright © 2016年 metasoft. All rights reserved.
//

#ifndef Random_hpp
#define Random_hpp

#include <stdlib.h>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

namespace codechiev {
    namespace base {
        
        class Random
        {
        public:
            typedef boost::random::mt19937 generator_type;
            
            template <typename T>
            static inline T GetRandomInt(T min, T max)
            {
                boost::random::uniform_int_distribution<> dist(min, max);
                return dist(Random::GetGenerator());
            }
            
            template <typename T>
            static inline T GetRandomNumber(T min, T max)
            {
                boost::random::uniform_real_distribution<> dist;
                return min + max * dist(Random::GetGenerator());
            }
            
        private:
            static generator_type &GetGenerator();
        };
        
        template<typename T>
        inline T random(T min, T max) {
            return Random::GetRandomInt(min, max);
        }
        
        template<>
        inline float random(float min, float max) {
            return Random::GetRandomNumber(min, max);
        }
        
        template<>
        inline long double random(long double min, long double max) {
            return Random::GetRandomNumber(min, max);
        }
        
        template<>
        inline double random(double min, double max) {
            return Random::GetRandomNumber(min, max);
        }
    }
}

#endif /* Random_hpp */
